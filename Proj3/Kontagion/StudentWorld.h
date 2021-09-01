#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Socrates;
class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
    void addDirt();
    Socrates* m_socratesPointer;
    std::vector<Actor*> m_vecOfActors;
};

double euclideanDistance(double x0, double y0, double x1, double y1);

#endif // STUDENTWORLD_H_
