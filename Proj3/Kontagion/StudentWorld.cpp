#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <cmath>
#include <string>
#include <vector>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() { cleanUp(); }

void StudentWorld::addDirt()
{
    int dirtNum = max(180 - (20 * getLevel()), 20);
    double randX, randY;
    while (dirtNum > 0)
    {
        randX = randY = 10000;
        while (euclideanDistance(VIEW_WIDTH / 2, VIEW_HEIGHT / 2, randX, randY) > 120.0)
        {
            randX = randInt(0, 2 * VIEW_RADIUS);
            randY = randInt(0, 2 * VIEW_RADIUS);
        }
        m_vecOfActors.push_back(new Dirt(randX, randY));
        dirtNum--;
    }
}

int StudentWorld::init()
{
    m_socratesPointer = new Socrates(this);
    addDirt();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    /*if (m_socratesPointer->isAlive())
        m_socratesPointer->doSomething();*/
    //decLives();
    for (int i = 0; i < m_vecOfActors.size(); i++)
        m_vecOfActors[i]->doSomething();
    m_socratesPointer->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_socratesPointer;
    for (int i = 0; i < m_vecOfActors.size(); i++)
    {
        delete m_vecOfActors[i];
        m_vecOfActors.erase(m_vecOfActors.begin() + i);
    }

}

double euclideanDistance(double x0, double y0, double x1, double y1)
{
    return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
}
