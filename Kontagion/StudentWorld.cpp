#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), liveBacterium(0), livePits(0), socrates(nullptr), m_level(1){}

StudentWorld::~StudentWorld(){cleanUp();}

//Add L pits to the Petri dish at random locations, in a manner such that no two pits
//overlap with each other (their centers are at least 2*SPRITE_RADIUS pixels
//apart from each other). Each pit must be no more 120 pixels from the center of the
//Petri dish which is at (VIEW_WIDTH/2, VIEW_HEIGHT/2).
//3. Add min(5 * L, 25) food objects to the Petri dish at random locations, in a manner
//such that no two food objects overlap with each other or previously-placed
//pits (their centers must be at least 2*SPRITE_RADIUS pixels apart from each
//other). Each food object must be no more 120 pixels from the center of the Petri
//dish which is at (VIEW_WIDTH/2, VIEW_HEIGHT/2).


int StudentWorld::init()
{
    m_level = getLevel();
    liveBacterium = 0;
    socrates = new Socrates(this, 100);
    // add static actors
    addPits();
    addFood();
    addDirt();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    if(!socrates->isAlive())
        return GWSTATUS_PLAYER_DIED;
    socrates->doSomething();
    
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end())
    {
        if ((*it)->isAlive())
        {
            (*it)->doSomething();
            if (!socrates->isAlive())
                return GWSTATUS_PLAYER_DIED;
        }
        it++;
    }
    // Remove newly-dead actors after each tick
    cleanActors();
    // Potentially add new actors to the game (e.g., goodies or fungi)
    addNewActors();
    // Update the Game Status Line
    updateDisplayText();
    
    if (liveBacterium == 0 && livePits == 0)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::updateDisplayText()
{
    ostringstream oss;
    oss.fill('0');
    if(getScore() < 0) // negative format conditional
        oss << "Score: -";
    else
        oss << "Score: ";
    oss << setw(6) << abs(getScore()) << "  Level: " << m_level << "  Lives: " << socrates->getLives() << "  Health: " << socrates->getHp() << "  Sprays: " << socrates->getSprays() << "  Flames: " << socrates->getFlames();
    string s = oss.str();
    setGameStatText(s);
}

void StudentWorld::addNewActors()
{
    int x, y = 0;
    // Fungus
    int lifetime = max(rand() % (300 - 10 * m_level), 50);
    if(genGoodieCoords(x, y, max(510 - (m_level * 10), 200)))
        actors.push_back(new Fungus(x, y, this, lifetime));
    
    // Goodie
    if(genGoodieCoords(x, y, max(510 - (m_level * 10), 250)))
    {
        int val = rand() % 100 + 1; // [1,100]
        if(val <= 60)
            actors.push_back(new HealthPack(x, y, this, lifetime));
        else if(60 < val && val <= 90)
            actors.push_back(new FlamePack(x, y, this, lifetime));
        else if(90 < val && val <= 100)
            actors.push_back(new LifePack(x, y, this, lifetime));
    }
}

// sets x, y to valid coords if chance is met
bool StudentWorld::genGoodieCoords(int &x, int &y, int chance)
{
    const double PI = 4 * atan(1);
    if(randInt(0, chance) == 0)
    {
        double posAngle = randInt(0, 360) * 1.0;
        x = VIEW_WIDTH/2 + VIEW_RADIUS * cos(posAngle/ 360 * 2 * PI);
        y = VIEW_HEIGHT/2 + VIEW_RADIUS * sin(posAngle/ 360 * 2 * PI);
        return true;
    }
    return false;
}


void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end())
    {
        delete *it;
        it++;
    }
    actors.clear();
    delete socrates;
}

void StudentWorld::addPits()
{
    livePits = m_level;
    for(int i = 0; i < livePits; i++)
    {
        int x, y;
        validCoords(x, y);
        for(Actor* ap : actors)
            while(distance(x, y, ap->getX(), ap->getY()) < 2 * SPRITE_WIDTH)
                validCoords(x, y);
        actors.push_back(new BacteriumPit(x,y,this));
    }
}

void StudentWorld::addFood()
{
    for(int i = 0; i <  min(5 * m_level, 25); i++)
    {
        int x, y;
        validCoords(x, y);
        for(Actor* ap : actors)
            while(distance(x, y, ap->getX(), ap->getY()) <= 2 * SPRITE_WIDTH)
                validCoords(x, y);
        actors.push_back(new Food(x,y,this));
    }
}

void StudentWorld::addDirt()
{
    for(int i = 0; i < max(180 - (20 * m_level), 20); i++)
    {
        int x, y;
        validCoords(x, y);
        for(Actor* ap : actors)
        {
            if(!ap->isDirt())
            {
                while(distance(x, y, ap->getX(), ap->getY()) < 2 * SPRITE_WIDTH)
                    validCoords(x, y);
            }
        }
        actors.push_back(new Dirt(x,y,this));
    }
}

// returns distance between coords
double StudentWorld::distance(double x1, double y1, double x2, double y2) const
{
    return std::sqrt(std::pow((x1-x2), 2) + std::pow((y1-y2), 2));
}

//
void StudentWorld::validCoords(int& x, int& y)
{
    x = randInt(0,VIEW_WIDTH);
    y = randInt(0, VIEW_HEIGHT);
    while(distance(x,y,VIEW_WIDTH/2, VIEW_HEIGHT/2) > 120)
    {
        x = randInt(0,VIEW_WIDTH);
        y = randInt(0, VIEW_HEIGHT);
    }
}

bool StudentWorld::damageActors(Projectile* proj)
{
    for(Actor* ap: actors)
    {
        if(ap->isDamageable() && ap->overlaps(proj))
        {
            // destroy static objects
            if(ap->isDirt() || ap->isGoodie())
                ap->setAlive(false);
            else
            {
                // damage and potentially destroy bacterium
                ap->addHp(-1 * proj->getDamage());
                if(ap->isAlive())
                    playSound(SOUND_SALMONELLA_HURT);
                else
                {
                    playSound(SOUND_SALMONELLA_DIE);
                    increaseScore(100);
                    if(randInt(0, 1) == 0)
                        actors.push_back(new Food(ap->getX(),ap->getY(),this));
                }
            }
            proj->setAlive(false);
            return true;
        }
    }
    return false;
}

// delete all dead actors and update counters
void StudentWorld::cleanActors()
{
    list<Actor*>::iterator it = actors.begin();
    while(it != actors.end())
        if(!(*it)->isAlive())
        {
            if((*it)->isBacterium())
                liveBacterium--;
            if((*it)->isBacteriumPit())
                livePits--;
            delete *it;
            it = actors.erase(it);
        }
        else
            it++;
}

bool StudentWorld::overlapsFood(Actor *a) const
{
    for(Actor* ap : actors)
        if(ap->isFood() && ap->overlaps(a))
        {
            ap->setAlive(false);
            return true;
        }
    return false;
}

bool StudentWorld::overlapsDirt(Actor *a) const
{
    for(Actor* ap : actors)
        if(ap->isDirt() && ap->moveOverlaps(a))
        {
            return true;
        }
    return false;
}

// sets x, y to coordinates of the closest food object, returns true if thi process was successful, false otherwise
bool StudentWorld::getClosestFood(Actor * a, double &x, double &y)
{
    int min = VIEW_HEIGHT;
    for(Actor* ap: actors)
    {
        if(ap->isFood())
        {
            double dist = distance(a->getX(), a->getY(), ap->getX(), ap->getY());
            if(dist <= 128 && dist < min)
            {
                min = dist;
                x = ap->getX();
                y = ap->getY();
            }
        }
    }
    return min != VIEW_HEIGHT;
}

// returns true if withing the tracking range of player
bool StudentWorld::nearSocrates(Actor* a, double dist) const
{
    return distance(a->getX(), a->getY(), socrates->getX(), socrates->getY()) <= dist;
}

// returns the angle to the player actor
int StudentWorld::directionToSocrates(Actor *a)
{
    const double PI = 4 * atan(1);
    double deltaY = socrates->getY() - a->getY();
    double deltaX = socrates->getX() - a->getX();
    return atan2(deltaY,deltaX) * 180/PI;
}
