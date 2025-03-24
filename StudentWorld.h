#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>


using namespace std;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    virtual ~StudentWorld();
    bool isValidPosition(int x, int y) const;
    bool canClimbAt(int x, int y) const;

    Player* getPlayer() const { return m_player; }
    
    void setDisplayText();
    vector<Actor*> getActors() {return actors;}
    
    void addActor(Actor* actor);
    
    Actor* getEnemy(int x, int y) const;
    bool isBonfire(int x, int y) const;
private:
    Player* m_player;
    vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_
