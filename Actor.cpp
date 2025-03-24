#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"


void LivingActor::fall() {
    if (getWorld()->isValidPosition(getX(), getY() - 1) && !getWorld()->canClimbAt(getX(), getY() - 1) && !getWorld()->canClimbAt(getX(), getY())) {
        moveTo(getX(), getY() - 1);
        return;
    }
}

void Player::doSomething() {

    //if dead, just return
    if (!isAlive()) {return;}
    
    // if frozen
    if(m_frozenTicks > 0) {
        if(m_isJumping) {
            handleJump();
            return;
        }
        m_frozenTicks--;
        return;
    }
    
    //jumping
    if (m_isJumping) {
        handleJump();
        return;
    }
    
    // falling
    fall();
    
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
         case KEY_PRESS_LEFT:
            if(getWorld()->isValidPosition(getX()-1, getY()) && getDirection() == left) {
                moveTo(getX()-1, getY());
            } else {
                setDirection(left);
            }
            break;
                
        case KEY_PRESS_RIGHT:
            if(getWorld()->isValidPosition(getX()+1, getY()) && getDirection() == right) {
                moveTo(getX()+1, getY());
            } else {
                setDirection(right);
            }
            break;
            
        case KEY_PRESS_UP:
            if (getWorld()->isValidPosition(getX(), getY() + 1) && getWorld()->canClimbAt(getX(), getY())) {
                moveTo(getX(), getY() + 1);
            }
            break;
            
        case KEY_PRESS_DOWN:
            if (getWorld()->isValidPosition(getX(), getY() - 1) && (getWorld()->canClimbAt(getX(), getY()) || getWorld()->canClimbAt(getX(), getY() -1 ))) {
                moveTo(getX(), getY() - 1);
            }
            break;
            
        case KEY_PRESS_SPACE:
            if (!m_isJumping) {
                m_isJumping = true;
                m_jumpStep = 0;
                setDirection(getDirection());
                getWorld()->playSound(SOUND_JUMP);
            }
            break;
            
        case KEY_PRESS_TAB:
            createBurp();
            break;
                
        default:
            break;
        // etc…
        }
    }
}

void Player::attack() {
    getWorld()->playSound(SOUND_PLAYER_DIE);
    getWorld()->decLives();
    setDead();
}

void Player::freeze() {
    m_frozenTicks = 50;
}

void Player::handleJump() {
    int newX = getX();
    int newY = getY();
    
    if (m_jumpStep == 0) {
        newY++;
    } else if (m_jumpStep >= 1 && m_jumpStep <= 3) {
        if(getDirection() == right) {
            newX++;
        } else {newX--;}
    } else if (m_jumpStep == 4) {
        newY--;
    }
    
    if(!getWorld()->isValidPosition(newX, newY)) {
        m_isJumping = false;
        return;
    }
    
    moveTo(newX, newY);
    m_jumpStep++;
    
    if(getWorld()->canClimbAt(getX(), getY())) {
        m_isJumping = false;
    }
    
    if(m_jumpStep > 4) {
        m_isJumping = false;
    }
}

void Player::createBurp()
{
    if (m_burps > 0) {
        int burpX = getX();
        int burpY = getY();
        
        switch (getDirection())
        {
            case right:
                burpX++;
                break;
            case left:
                burpX--;
                break;
            default:
                break;
        }
        
        if (getWorld()->isValidPosition(burpX, burpY))
        {
            getWorld()->addActor(new Burp(getWorld(), burpX, burpY));
            getWorld()->playSound(SOUND_BURP);
        }
        
        m_burps--;
    }
}

void Player::levelUp() {
    getWorld()->increaseScore(1000);
    getWorld()->playSound(SOUND_FINISHED_LEVEL);
    m_levelFinished = true;
}


void Bonfire::doSomething() {
    increaseAnimationNumber();
    
    if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
        getWorld()->getPlayer()->attack(); 
    }
}

void ExtraLifeGoodie::doSomething() {
    if(!isAlive()) {
        return;
    } else {
        if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
            getWorld()->increaseScore(50);
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->incLives();
        }
    }
}

void Garlic::doSomething() {
    if(!isAlive()) {
        return;
    } else {
        if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
            getWorld()->increaseScore(25);
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->getPlayer()->incBurps(5);
        }
    }
}

void Burp::doSomething() {
    increaseTicks();
        
    m_burpTime--;
    
    if(m_burpTime <= 0) {
        setDead();
    }
    Actor* enemy = getWorld()->getEnemy(getX(), getY());
    if(enemy != nullptr)
        enemy->isAttacked();
}

void Fireball::doSomething() {
    increaseTicks();
    
    if(!isAlive()) {
        return;
    }
    
    if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
        getWorld()->getPlayer()->attack();
        return;
    }
    
    if(ticksDone()%10 == 0)
    {
        if(getWorld()->canClimbAt(getX(), getY()) && getWorld()->isValidPosition(getX(), getY() + 1) && !m_climbDownState)
        {
            if (m_climbUpState || randInt(1,3) == 1)
            {
                setUpClimb();
                moveTo(getX(), getY() + 1);
                return;
            }
        }
        
        else if (getWorld()->canClimbAt(getX(), getY() -1) && !m_climbUpState)
            {
                if (m_climbDownState || randInt(1,3) == 1)
                {
                    setDownClimb();
                    moveTo(getX(), getY() - 1);
                    return;
                }
            }
        
        if (climbState() && (getWorld()->isValidPosition(getX(), getY() + 1) || getWorld()->isValidPosition(getX(), getY() - 1)))
            {
                m_climbUpState = false;
                m_climbDownState = false;
            }

        if (!climbState()) {
                if (getDirection() == left) {
                    if (!(getWorld()->isValidPosition(getX()-1, getY())) || (getWorld()->isValidPosition(getX()-1, getY()-1) && !(getWorld()->canClimbAt(getX()-1, getY()-1)))) {
                        setDirection(right);
                    } else {
                        moveTo(getX() - 1, getY());
                    }
                } else {
                    if (!(getWorld()->isValidPosition(getX()+1, getY())) || (getWorld()->isValidPosition(getX()+1, getY()-1) && !(getWorld()->canClimbAt(getX()+1, getY()-1)))){
                        setDirection(left);
                    } else {
                        moveTo(getX() + 1, getY());
                    }
                }
            }
        }
    
        if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
            getWorld()->getPlayer()->attack();
            return;
    }
}

void Fireball::isAttacked()
{
    setDead();
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(100);
    if (randInt(1,3) == 1) {
        getWorld()->addActor(new Garlic(getWorld(), getX(), getY()));
    }
}

void Kong::doSomething()
{
    increaseTicks();
    if(!isAlive()) {
        return;
    }
    
    increaseAnimationNumber();
    
    int dx = getX() - getWorld()->getPlayer()->getX();
    int dy = getY() - getWorld()->getPlayer()->getY();
    double distance = sqrt(dx * dx + dy * dy);
    
    if (distance <= 2) {
        m_flee = true;
    }
    
    int N;
    if (200 - 50 * getWorld()->getLevel() < 50) {
        N = 50;
    } else N = 200 - 50 * getWorld()->getLevel();
    if (!m_flee && ticksDone() % N == 0) {
        getWorld()->addActor(new Barrel(getWorld(), getX(), getY(), getDirection()));
    }
    
    if(ticksDone() % 5 == 0) {
        if(m_flee) {
            if(getWorld()->canClimbAt(getX(), getY() + 1) || getY() + 1 == VIEW_HEIGHT) {
                moveTo(getX(), getY() + 1);
            }
        }
        if(getY() == VIEW_HEIGHT) {
            getWorld()->getPlayer()->levelUp();
        }
    }
}

void Barrel::doSomething()
{
    increaseTicks();
    if(!isAlive()) {
        return;
    }
    
    if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
        getWorld()->getPlayer()->attack();
        return;
    }
    
    if(getWorld()->isBonfire(getX(), getY())) {
        setDead();
        return;
    }
    
    bool justFell = false;
    if(getWorld()->isValidPosition(getX(), getY() - 1)) {
        moveTo(getX(), getY() - 1);
        if(!getWorld()->isValidPosition(getX(), getY() - 1)) {justFell = true;}
    }
    
    if(justFell) {
        if(getDirection() == left) {
            setDirection(right);
        } else {setDirection(left);}
        justFell = false;
    }
    
    if(ticksDone() % 10 == 0) {
        if (getDirection() == left) {
            if (!getWorld()->isValidPosition(getX()-1, getY())) {
                setDirection(right);
            } else {
                moveTo(getX() - 1, getY());
            }
        } else {
            if (!getWorld()->isValidPosition(getX()+1, getY())){
                setDirection(left);
            } else {
                moveTo(getX() + 1, getY());
            }
        }
    }
    
    if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()) {
        getWorld()->getPlayer()->attack();
        return;
    }
}

void Barrel::isAttacked()
{
    setDead();
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(100);
}


void Koopa::doSomething()
{
    increaseTicks();
    if(!isAlive()) {
        return;
    }
    
    if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY() && m_timer == 0) {
        getWorld()->getPlayer()->setFrozen(50);
        m_timer = 50;
        return;
    }
    
    if(m_timer <= 50 && m_timer > 0) {
        m_timer--;
    }
    
    if(ticksDone() % 10 == 0) {
        if (getDirection() == left) {
            if (!(getWorld()->isValidPosition(getX()-1, getY())) || (getWorld()->isValidPosition(getX()-1, getY()-1) && !(getWorld()->canClimbAt(getX()-1, getY()-1)))) {
                setDirection(right);
            } else {
                moveTo(getX() - 1, getY());
            }
        } else {
            if (!(getWorld()->isValidPosition(getX()+1, getY())) || (getWorld()->isValidPosition(getX()+1, getY()-1) && !(getWorld()->canClimbAt(getX()+1, getY()-1)))) {
                setDirection(left);
            } else {
                moveTo(getX() + 1, getY());
            }
        }
        
        
        if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY() && m_timer == 0) {
            getWorld()->getPlayer()->setFrozen(50);
            m_timer = 50;
            return;
        }
    }
}

void Koopa::isAttacked()
{
    setDead();
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(100);
    if (randInt(1,3) == 1) {
        getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
    }
}
