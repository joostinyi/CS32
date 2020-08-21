#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Socrates::doSomething()
{
    if(!isAlive())
        return;
    int ch;
    if (getSW()->getKey(ch))
    {
        // user hit a key during this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                move(5);
                break;
            case KEY_PRESS_RIGHT:
                move(-5);
                break;
            case KEY_PRESS_SPACE:
                if(getSprays() > 0)
                {
                    double x, y = 0;
                    getPositionInThisDirection(getDirection(), 2*SPRITE_RADIUS, x, y);
                    getSW()->addActor(new Spray(x, y, getSW(), getDirection()));
                    m_sprays--;
                    getSW()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            case KEY_PRESS_ENTER:
                if(getFlames() > 0)
                {
                    int deg = getDirection();
                    for(int i = 0; i < 16; i++, deg = (deg + 22) % 360) // disperse flames
                    {
                        double x, y = 0;
                        getPositionInThisDirection(deg, 2*SPRITE_RADIUS, x, y);
                        getSW()->addActor(new Flame(x, y, getSW(), deg));
                    }
                    getSW()->playSound(SOUND_PLAYER_FIRE);
                    m_flames--;
                }
                break;
        }
    }
    else if(m_sprays < 20)
        m_sprays++;
}

void Socrates::move(int angle)
{
    const double PI = 4 * atan(1);
    m_posAngle+= angle;
    double newX = VIEW_WIDTH/2 + VIEW_RADIUS * cos(m_posAngle/ 360 * 2 * PI);
    double newY = VIEW_HEIGHT/2 + VIEW_RADIUS * sin(m_posAngle/ 360 * 2 * PI);
    moveTo(newX, newY);
    setDirection(m_posAngle + 180);
}

void Projectile::doSomething()
{
    if(!isAlive())
        return;
    if(!getSW()->damageActors(this))
    {
        moveForward(2*SPRITE_RADIUS);
        m_distance += 2*SPRITE_RADIUS;
        if(m_distance == m_max)
            setAlive(false);
    }
    else
        setAlive(false);
    return;
}

bool Goodie::consumed(int points)
{
    if(getUptime() == getLifetime())
    {
        setAlive(false);
    }
    addtime();
    int sound = SOUND_GOT_GOODIE;
    if(!isAlive())
        return false;
    if(getSW()->overlapsSocrates(this))
    {
        getSW()->increaseScore(points);
        setAlive(false);
        if(isFungus())
            sound = SOUND_PLAYER_HURT;
        getSW()->playSound(sound);
        return true;
    }
    return false;
}

void HealthPack::doSomething()
{
    if(consumed(getPoints()))
        getSW()->restoreHealth();
}

void LifePack::doSomething()
{
    if(consumed(getPoints()))
        getSW()->incLives();
}

void FlamePack::doSomething()
{
    if(consumed(getPoints()))
        getSW()->addFlames();
}

void Fungus::doSomething()
{
    if(consumed(getPoints()))
        getSW()->addHealth(-20);
}

void BacteriumPit::doSomething()
{
    if(empty())
        setAlive(false);
    if(rand() % 50 == 0)
    {
        string bKey = "";
        while(bKey == "") // goes through available items in the map until bacterium, not all expended, is spawned
            switch(randInt(0, 2))
            {
                case 0:
                    bKey = "Salmonella";
                    if(bacteria.at(bKey) != 0)
                    {
                        getSW()->addActor(new Salmonella(getX(), getY(), getSW()));
                        getSW()->playSound(SOUND_BACTERIUM_BORN);
                        getSW()->incrementBacterium();
                        bacteria["Salmonella"]--;
                    }
                    else
                        bKey = "";
                    break;
                case 1:
                    bKey = "AggroSalmonella";
                    if(bacteria.at(bKey) != 0)
                    {
                        getSW()->addActor(new AggroSalmonella(getX(), getY(), getSW()));
                        getSW()->playSound(SOUND_BACTERIUM_BORN);
                        getSW()->incrementBacterium();
                        bacteria["AggroSalmonella"]--;
                    }
                    else
                        bKey = "";
                    break;
                case 2:
                    bKey = "EColi";
                    if(bacteria.at(bKey) != 0)
                    {
                        getSW()->addActor(new EColi(getX(), getY(), getSW()));
                        getSW()->playSound(SOUND_BACTERIUM_BORN);
                        getSW()->incrementBacterium();
                        bacteria["EColi"]--;
                    }
                    else
                        bKey = "";
                    break;
            }
    }
}

void Salmonella::doSomething()
{
    if(!isAlive())
        return;
    if(!damageSocrates())
    {
        if(getFoodEaten() == 3)
            duplicate();
        else if(getSW()->overlapsFood(this))
            setFoodEaten(getFoodEaten() + 1);
    }
    if(getMovePlanDist() > 0)
        move();
    else
        setFoodAngle();
}

void AggroSalmonella::doSomething()
{
    if(!isAlive())
        return;
    bool changeMove = true;
    if(getSW()->nearSocrates(this, 72))
    {
        setDirection(getSW()->directionToSocrates(this));
        canMoveForward();
        changeMove = false;
    }
    if(!damageSocrates())
    {
        if(getFoodEaten() == 3)
            duplicate();
        else if(getSW()->overlapsFood(this))
            setFoodEaten(getFoodEaten() + 1);
    }
    if(!changeMove)
        return;
    if(getMovePlanDist() > 0)
        move();
    else
        setFoodAngle();
}

void EColi::doSomething()
{
    if(!isAlive())
        return;
    if(!damageSocrates())
    {
        if(getFoodEaten() == 3)
            duplicate();
        else if(getSW()->overlapsFood(this))
            setFoodEaten(getFoodEaten() + 1);
    }
    if(getSW()->nearSocrates(this, 256))
    {
        setDirection(getSW()->directionToSocrates(this));
        for(int i = 0; i < 10; i++)
        {
            if(canMoveForward())
                return;
            else
                setDirection(getDirection() + 10);
        }
    }
}

bool Bacterium::damageSocrates()
{
    if(getSW()->overlapsSocrates(this))
    {
        getSW()->damageSocrates(m_damage);
        return true;
    }
    return false;
}

// sets x, y to valid spawning coordinates
void Bacterium::getNewCoords(int& x, int& y)
{
    int mult = 0;
    if(x < VIEW_WIDTH/2)
        mult = 1;
    else if(x > VIEW_WIDTH/2)
        mult = -1;
    x = getX() + mult * SPRITE_WIDTH/2;
    
    mult = 0;
    if(y < VIEW_HEIGHT/2)
        mult = 1;
    else if(y > VIEW_HEIGHT/2)
        mult = -1;
    y = getY() + mult * SPRITE_WIDTH/2;
}

void Bacterium::move()
{
    m_movePlanDist--;
    if(!canMoveForward())
    {
        setDirection(randInt(0, 359));
        m_movePlanDist = 10;
    }
    return;
}

// if able, issues move to bacterium actor and returns true
bool Bacterium::canMoveForward()
{
    moveForward(3);
    if(getSW()->overlapsDirt(this) || getSW()->distance(getX(), getY(), VIEW_WIDTH/2, VIEW_HEIGHT/2) > VIEW_RADIUS)
    {
        moveForward(-3);
        return false;
    }
    return true;
}

// sets actor's angle to the nearest food object
void Bacterium::setFoodAngle()
{
    const double PI = 4 * atan(1);
    double x, y;
    if(getSW()->getClosestFood(this, x, y))
    {
        double deltaY = y - getY();
        double deltaX = x - getX();
        setDirection(atan2(deltaY,deltaX) * 180/PI);
        if(!canMoveForward())
        {
            setDirection(randInt(0, 359));
            m_movePlanDist = 10;
        }
    }
    else
    {
        setDirection(randInt(0, 359));
        m_movePlanDist = 10;
    }
}

void Salmonella::duplicate()
{
    int x, y = 0;
    getNewCoords(x, y);
    getSW()->addActor(new Salmonella(x, y, getSW()));
    getSW()->incrementBacterium();
    setFoodEaten(0);
}

void AggroSalmonella::duplicate()
{
    int x, y = 0;
    getNewCoords(x, y);
    getSW()->addActor(new AggroSalmonella(x, y, getSW()));
    getSW()->incrementBacterium();
    setFoodEaten(0);
}

void EColi::duplicate()
{
    int x, y = 0;
    getNewCoords(x, y);
    getSW()->addActor(new EColi(x, y, getSW()));
    getSW()->incrementBacterium();
    setFoodEaten(0);
}

