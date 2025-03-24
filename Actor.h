#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection = none) :
            GraphObject(imageID, startX, startY, startDirection), m_world(world) {}
    
    virtual void doSomething() = 0;
    virtual bool blocksMovement() const {return false;}
    virtual bool canBeClimbed() const { return false; }
    virtual bool isAlive() const { return true; }
    virtual void setDead() {return;}
    virtual bool canDie() const {return false;}
    virtual int ticksDone() const {return m_ticks;}
    virtual int increaseTicks() {return m_ticks++;}
    
    virtual void isAttacked() = 0;
    virtual bool isEnemy() const {return false;}
    
    
    StudentWorld* getWorld() const {return m_world;}
private:
    StudentWorld* m_world;
    int m_ticks;
};

class Floor : public Actor
{
public:
    Floor(StudentWorld* world, double startX, double startY) :
        Actor(world, IID_FLOOR, startX, startY, 0) {}

    virtual void doSomething() override { return;}
    virtual bool blocksMovement() const override {return true;}
    virtual void isAttacked() override {}
private:
};

class Ladder : public Actor
{
public:
    Ladder(StudentWorld* world, double startX, double startY) :
        Actor(world, IID_LADDER, startX, startY, 0) {}

    virtual void doSomething() override { return;}
    virtual bool blocksMovement() const override {return false;}
    virtual bool canBeClimbed() const override { return true; }
    virtual void isAttacked() override {}
private:
};

class Bonfire : public Actor
{
public:
    Bonfire(StudentWorld* world, double startX, double startY)
        : Actor(world, IID_BONFIRE, startX, startY, none) {}

    virtual void doSomething() override;
    virtual bool blocksMovement() const override { return false;}
    virtual void isAttacked() override {}
private:
};


// Living Actors Start

class LivingActor : public Actor
{
public:
    LivingActor(StudentWorld* world, int imageID, double startX, double startY, int startDirection) :
        Actor(world, imageID, startX, startY, startDirection), m_alive(true) {}
    
    virtual bool isAlive() const override { return m_alive;}
    virtual void setDead() override {m_alive = false; return;}
    virtual bool canDie() const override {return true;}
    virtual void isAttacked() override {}
    
    void fall();
    
private:
    bool m_alive;
};

class Player : public LivingActor
{
public:
    Player(StudentWorld* world, double startX, double startY) :
    LivingActor(world, IID_PLAYER, startX, startY, right), m_lives(3), m_burps(0), m_isJumping(false), m_jumpStep(0), m_frozenTicks(0), m_levelFinished(false) {}
    
    virtual void doSomething() override;
    virtual bool blocksMovement() const override {return true;}
    virtual void isAttacked() override {}
    
    void attack();
    void freeze();
        
    int getLives() const { return m_lives; }
    int getBurps() const { return m_burps; }
    int incBurps(int x) {return m_burps = m_burps + x;}
    
    void levelUp();
    bool islevelfinished() const{return m_levelFinished;}
    void createBurp();
    
    void setFrozen(int x) {m_frozenTicks += x;}
private:
    int m_lives;
    int m_burps;
    bool m_isJumping;
    int m_jumpStep;
    int m_frozenTicks;
    bool m_levelFinished;
    
    void handleJump();
};

class ExtraLifeGoodie : public LivingActor
{
public:
    ExtraLifeGoodie(StudentWorld* world, double startX, double startY)
        : LivingActor(world, IID_EXTRA_LIFE_GOODIE, startX, startY, none) {}
    
    virtual void doSomething() override;
    virtual void isAttacked() override {}

private:
};

class Garlic : public LivingActor
{
public:
    Garlic(StudentWorld* world, double startX, double startY)
        : LivingActor(world, IID_GARLIC_GOODIE, startX, startY, none) {}
    
    virtual void doSomething() override;
    virtual void isAttacked() override {}

private:
};

class Burp : public LivingActor
{
public:
    Burp(StudentWorld* world, double startX, double startY)
         : LivingActor(world, IID_BURP, startX, startY, none), m_burpTime(5) {}
    
    virtual void doSomething() override;
    virtual void isAttacked() override {}
    
private:
    int m_burpTime;
};

class Fireball : public LivingActor
{
public:
    Fireball(StudentWorld* world, double startX, double startY)
    : LivingActor(world, IID_FIREBALL, startX, startY, randInt(1,2) == 1 ? left : right), m_climbUpState(false), m_climbDownState(false) {}
    
    virtual void doSomething() override;
    virtual void isAttacked() override;
    virtual bool isEnemy() const override {return true;}
private:
    bool m_climbUpState;
    bool m_climbDownState;
    
    virtual bool climbState() const {return m_climbUpState || m_climbDownState;}
    virtual void setUpClimb() {m_climbUpState = true;}
    virtual void setDownClimb() {m_climbDownState = true;}
};

class Kong : public LivingActor
{
public:
    Kong(StudentWorld* world, double startX, double startY, int startDirection)
    : LivingActor(world, IID_KONG, startX, startY, startDirection), m_flee(false) {}
    
    virtual void doSomething() override;
private:
    bool m_flee;
};

class Barrel : public LivingActor
{
public:
    Barrel(StudentWorld* world, double startX, double startY, int startDirection)
    : LivingActor(world, IID_BARREL, startX, startY, startDirection) {}
    
    virtual void doSomething() override;
    virtual void isAttacked() override;
    virtual bool isEnemy() const override {return true;}
private:
    
};

class Koopa : public LivingActor
{
public:
    Koopa(StudentWorld* world, double startX, double startY)
    : LivingActor(world, IID_KOOPA, startX, startY, randInt(1,2) == 1 ? left : right), m_timer(0) {}
    
    virtual void doSomething() override;
    virtual void isAttacked() override;
    virtual bool isEnemy() const override {return true;}
private:
    int m_timer;
};


#endif // ACTOR_H_
