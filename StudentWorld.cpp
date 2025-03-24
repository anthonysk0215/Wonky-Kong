#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_player = nullptr;
    actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    
    int currentLevel = getLevel();
    string levelFile = "";
    if (getLevel() < 10) {
        levelFile = "level0" + to_string(currentLevel) + ".txt";
    } else {levelFile = "level" + to_string(currentLevel) + ".txt";}
    
    Level::LoadResult result = lev.loadLevel(levelFile);
        
    if (result == Level::load_fail_file_not_found) {
            return GWSTATUS_PLAYER_WON;
        } else if (result == Level::load_fail_bad_format) {
            return GWSTATUS_LEVEL_ERROR;
        } else if (result == Level::load_success) {
            for (int x = 0; x < VIEW_WIDTH; x++) {
                for (int y = 0; y < VIEW_HEIGHT; y++) {
                    Level::MazeEntry item = lev.getContentsOf(x, y);
                    
                    if (!isValidPosition(x, y)) {
                        return GWSTATUS_LEVEL_ERROR;
                    }
                    switch(item) {
                        case Level::empty:
                            break;
                        case Level::player:
                            m_player = new Player(this, x, y);
                            break;
                        case Level::floor:
                            actors.push_back(new Floor(this, x, y));
                            break;
                        case Level::ladder:
                            actors.push_back(new Ladder(this, x, y));
                            break;
                        case Level::bonfire:
                            actors.push_back(new Bonfire(this, x, y));
                            break;
                        case Level::extra_life:
                            actors.push_back(new ExtraLifeGoodie(this, x, y));
                            break;
                        case Level::garlic:
                            actors.push_back(new Garlic(this, x, y));
                            break;
                        case Level::fireball:
                            actors.push_back(new Fireball(this, x, y));
                            break;
                        case Level::right_kong:
                            actors.push_back(new Kong(this, x, y, 0));
                            break;
                        case Level::left_kong:
                            actors.push_back(new Kong(this, x, y, 180));
                            break;
                        case Level::koopa:
                            actors.push_back(new Koopa(this, x, y));
                            break;
                            
                        default:
                            break;
                    }
                }
            }
        }

    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isValidPosition(int x, int y) const {
    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
        return false;

    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->getX() == x && actors[i]->getY() == y && actors[i]->blocksMovement()) {
            return false;
        }
    }

    return true;
}

bool StudentWorld::canClimbAt(int x, int y) const {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i]->getX() == x && actors[i]->getY() == y && actors[i]->canBeClimbed()) {
            return true;
        }
    }
    return false;
}

int StudentWorld::move()
{
    setDisplayText();

    m_player->doSomething();
    for(int i = 0; i < actors.size(); i++) {
        if (actors[i]->isAlive()) {
            actors[i]->doSomething();
        }
        
        if (!m_player->isAlive()) {
            return GWSTATUS_PLAYER_DIED;  // Signal to restart the level or end the game
        }
    }
    
    for (int i = 0; i < actors.size(); ) {
        if (!actors[i]->isAlive()) {
            delete actors[i];
            actors.erase(actors.begin() + i);
        } else {
            i++;
        }
    }
    
    
    if(getPlayer()->islevelfinished())
        return GWSTATUS_FINISHED_LEVEL;
    
    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    for(int i = 0; i < actors.size(); i++) {
        delete actors[i];
    }
    actors.clear();
    
    delete m_player;
    m_player = nullptr;
}

void StudentWorld::setDisplayText() {
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int burps = getPlayer()->getBurps();
    
    ostringstream oss;
    oss << "Score: " << setw(7) << setfill('0') << score << "  ";  // Score with leading zeros
    oss << "Level: " << level << "  ";
    oss << "Lives: " << lives << "  ";
    oss << "Burps: " << burps;
    
    setGameStatText(oss.str());
}

void StudentWorld::addActor(Actor* actor)
{
    actors.push_back(actor);
}

Actor* StudentWorld::getEnemy(int x, int y) const
{
    for(int k = 0; k < actors.size(); k++)
    {
        if(actors[k]->getX() == x && actors[k]->getY() == y)
        {
            if(actors[k]->isEnemy()) {return actors[k];}
        }
    }
    return nullptr;
}


bool StudentWorld::isBonfire(int x, int y) const
{
    string levelFile = "level0" + to_string(getLevel()) + ".txt";
    Level lev(assetPath());
    lev.loadLevel(levelFile);
    Level::MazeEntry item = lev.getContentsOf(x, y);
    return item == Level::bonfire;
}
