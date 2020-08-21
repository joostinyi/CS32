#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

using namespace std;

class GraphObject;
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
    Actor(int imageID, int x, int y, StudentWorld* sw, int startDirection, int depth)
    : GraphObject(imageID, x, y, startDirection, depth), m_alive(true), m_sw(sw){}
    virtual ~Actor(){};
    virtual void doSomething() = 0;
    virtual bool isAlive() const {return m_alive;}
    virtual bool isDirt() const {return false;}
    virtual bool isBacterium() const {return false;}
    virtual bool isDamageable() const {return false;}
    virtual bool isGoodie() const {return false;}
    virtual bool isSocrates() const {return false;}
    virtual bool isFungus() const {return false;}
    virtual bool isBacteriumPit() const {return false;}
    virtual bool isFood() const {return false;}
    void setAlive(bool result) {m_alive = result;}
    bool overlaps(Actor* a) const;
    bool moveOverlaps(Actor* a) const;
    virtual void addHp(int amount){};
    virtual int getHp(){return 0;};
    StudentWorld* getSW() const {return m_sw;}
private:
    bool m_alive;
    StudentWorld* m_sw;
};

class DynamicActor : public Actor
{
public:
    DynamicActor(int imageID, int x, int y, StudentWorld* sw, int startDirection, int depth, int hp)
    : Actor(imageID, x, y, sw, startDirection, depth), m_hp(hp){}
    virtual ~DynamicActor(){};
    void addHp(int amount);
    int getHp() const {return m_hp;}
    bool isDamageable() const {return true;}
private:
    int m_hp;
};

    class Socrates : public DynamicActor
    {
    public:
        Socrates(StudentWorld* sw, int hp)
        : DynamicActor(IID_PLAYER, 0, VIEW_HEIGHT/2, sw, 0, 0, hp), m_sprays(20), m_flames(5), m_posAngle(180), m_lives(3) {}
        ~Socrates(){};
        bool isSocrates() const {return true;}
        void doSomething();
        void addFlames() {m_flames += 5;}
        void addLives() {m_lives++;}
        int getLives() const {return m_lives;}
        int getSprays() const {return m_sprays;}
        int getFlames() const {return m_flames;}
    private:
        void move(int angle);
        int m_sprays;
        int m_flames;
        double m_posAngle;
        int m_lives;
    };

    class Bacterium : public DynamicActor
    {
    public:
        Bacterium(int imageID, int x, int y, StudentWorld* sw, int startDirection, int depth, int hp, int damage)
        : DynamicActor(imageID, x, y, sw, startDirection, depth, hp), m_movePlanDist(0), m_damage(damage), m_foodEaten(0){}
        virtual ~Bacterium(){};
        virtual void move();
        bool isBacterium() const {return true;}
        bool canMoveForward();
        int getMovePlanDist() const {return m_movePlanDist;}
        void setMovePlanDist(int amount)  {m_movePlanDist = amount;}
        void setFoodAngle();
        virtual void duplicate(){};
        bool damageSocrates();
        void setMovePlan();
        void getNewCoords(int& x, int& y);
        void setFoodEaten(int amount) {m_foodEaten = amount;}
        int getFoodEaten() {return m_foodEaten;}
    private:
        int m_movePlanDist;
        int m_damage;
        int m_foodEaten;
    };

        class Salmonella : public Bacterium
        {
        public:
            Salmonella(int x, int y, StudentWorld* sw)
            : Bacterium(IID_SALMONELLA, x, y, sw, 90, 0, 4, 1){}
            ~Salmonella(){};
            void doSomething();
            void duplicate();
        };

        class AggroSalmonella : public Bacterium
        {
        public:
            AggroSalmonella(int x, int y, StudentWorld* sw)
            : Bacterium(IID_SALMONELLA, x, y, sw, 90, 0, 10, 2){}
            ~AggroSalmonella(){};
            bool isAggro() const {return true;}
            void doSomething();
            void duplicate();
        };

        class EColi : public Bacterium
        {
        public:
            EColi(int x, int y, StudentWorld* sw)
            : Bacterium(IID_ECOLI, x, y, sw, 90, 0, 5, 4){}
            ~EColi(){};
            void doSomething();
            void duplicate();
        };

class Projectile : public Actor
{
public:
    Projectile(int imageID, int x, int y, StudentWorld* sw, int startDirection, int max, int damage)
    : Actor(imageID, x, y, sw, startDirection, 1), m_distance(0), m_max(max), m_damage(damage) {}
    virtual ~Projectile(){};
    int getDamage() const {return m_damage;}
    void doSomething();
private:
    int m_distance;
    int m_max;
    int m_damage;
};

        class Flame : public Projectile
        {
        public:
            Flame(int x, int y, StudentWorld* sw, int startDirection)
            : Projectile(IID_FLAME, x, y, sw, startDirection, 32, 5){}
            virtual ~Flame(){};
        private:
        };

        class Spray : public Projectile
        {
        public:
            Spray(int x, int y, StudentWorld* sw, int startDirection)
            : Projectile(IID_SPRAY, x, y, sw, startDirection, 112, 2){}
            virtual ~Spray(){};
        };

class StaticActor : public Actor
{
public:
    StaticActor(int imageID, int x, int y, StudentWorld* sw, int startDirection, int depth)
    : Actor(imageID, x, y, sw, startDirection, depth){}
    virtual ~StaticActor(){};
};

        class BacteriumPit : public StaticActor
        {
        public:
            BacteriumPit(int x, int y, StudentWorld* sw)
            : StaticActor(IID_PIT, x, y, sw, 0, 1), bacteria{{"Salmonella", 5}, {"AggroSalmonella", 3}, {"EColi", 2}}{};
            virtual ~BacteriumPit(){};
            bool isBacteriumPit() const {return true;}
            bool empty() const {return bacteria.at("Salmonella") == 0 && bacteria.at("AggroSalmonella") == 0 && bacteria.at("EColi") == 0;}
            void doSomething();
        private:
            map<string, int> bacteria;
        };

        class Dirt : public StaticActor
        {
        public:
            Dirt(int x, int y, StudentWorld* sw)
            : StaticActor(IID_DIRT, x, y, sw, 0, 1) {}
            ~Dirt(){};
            void doSomething() {return;}
            bool isDirt() const {return true;}
            bool isDamageable() const {return true;}
        };

        class Food : public StaticActor
        {
        public:
            Food(int x, int y, StudentWorld* sw)
            : StaticActor(IID_FOOD, x, y, sw, 90, 1) {}
            ~Food(){};
            bool isFood() const {return true;}
            void doSomething() {return;};
        };
        
        class Goodie : public StaticActor
        {
        public:
            Goodie(int imageID, int x, int y, StudentWorld* sw, int startDirection, int depth, int lifetime, int points)
            : StaticActor(imageID, x, y, sw, startDirection, depth), m_lifetime(lifetime), m_uptime(0), m_points(points){}
            virtual ~Goodie(){};
            bool isGoodie() const {return true;}
            bool isDamageable() const {return true;}
            int getPoints() const {return m_points;}
            void addtime() {m_uptime++;}
            int getUptime() const {return m_uptime;}
            int getLifetime() const {return m_lifetime;}
            bool consumed(int points);
        private:
            int m_lifetime;
            int m_uptime;
            int m_points;
        };

                class HealthPack : public Goodie
                {
                public:
                    HealthPack(int x, int y, StudentWorld* sw, int lifetime)
                    : Goodie(IID_RESTORE_HEALTH_GOODIE, x, y, sw, 0, 1, lifetime, 250){}
                    virtual ~HealthPack(){};
                    void doSomething();
                };

                class LifePack : public Goodie
                {
                public:
                    LifePack(int x, int y, StudentWorld* sw, int lifetime)
                    : Goodie(IID_EXTRA_LIFE_GOODIE, x, y, sw, 0, 1, lifetime, 500){}
                    virtual ~LifePack(){};
                    void doSomething();
                };

                class FlamePack : public Goodie
                {
                public:
                    FlamePack(int x, int y, StudentWorld* sw, int lifetime)
                    : Goodie(IID_FLAME_THROWER_GOODIE, x, y, sw, 0, 1, lifetime, 300){}
                    virtual ~FlamePack(){};
                    void doSomething();
                };

                class Fungus : public Goodie
                {
                public:
                    Fungus(int x, int y, StudentWorld* sw, int lifetime)
                    : Goodie(IID_FUNGUS, x, y, sw, 0, 1, lifetime, -50) {}
                    ~Fungus(){};
                    bool isFungus() const {return true;}
                    void doSomething();
                };

inline bool Actor::overlaps(Actor* a) const
{
    int x1 = getX();
    int x2 = a->getX();
    int y1 = getY();
    int y2 = a->getY();
    return sqrt(pow((x1-x2), 2) + std::pow((y1-y2), 2)) <= SPRITE_WIDTH;
}

inline bool Actor::moveOverlaps(Actor* a) const
{
    int x1 = getX();
    int x2 = a->getX();
    int y1 = getY();
    int y2 = a->getY();
    return sqrt(pow((x1-x2), 2) + std::pow((y1-y2), 2)) <= SPRITE_WIDTH/2;
}

inline void DynamicActor::addHp(int amount)
{
    m_hp += amount;
    if(m_hp <= 0)
        setAlive(false);
}

#endif // ACTOR_H_
