#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void addActor(Actor* a){actors.push_back(a);}
    void restoreHealth(){socrates->addHp(100 - socrates->getHp());}
    void addHealth(int amount){socrates->addHp(amount);}
    void addFlames(){socrates->addFlames();}
    bool damageActors(Projectile* proj); // returns true if proj damages an actor
    bool overlapsSocrates(Actor* a) const {return socrates->overlaps(a);}
    bool overlapsFood(Actor* a) const;
    bool overlapsDirt(Actor* a) const;
    void damageSocrates(int damage) {socrates->addHp(-damage);}
    double distance(double x1, double y1, double x2, double y2) const;
    bool getClosestFood(Actor* a, double& x, double& y); // changes x, y to coords of closest food and returns true, false otherwise
    bool nearSocrates(Actor* a, double distance) const;
    int directionToSocrates(Actor* a);
    void incrementBacterium() {liveBacterium++;}
private:
    void validCoords(int& x, int& y);
    void cleanActors();
    void addNewActors();
    bool genGoodieCoords(int& x, int& y, int chance); // returns true if world is to spawn goodie
    void updateDisplayText();
    void addDirt();
    void addPits();
    void addFood();
    Socrates* socrates;
    list<Actor*> actors;
    int liveBacterium;
    int livePits;
    int m_level;
};

#endif // STUDENTWORLD_H_
