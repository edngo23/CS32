#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, int depth)
		:GraphObject(imageID, startX, startY, dir, depth) { m_alive = true; }

	virtual ~Actor() {}

	virtual bool isAlive() const { return m_alive; }

	virtual void doSomething() = 0;

private:
	bool m_alive;
};

class ActiveActor : public Actor
{
public:
	ActiveActor(int imageID, double startX, double startY, Direction dir, int depth, int health)
		:Actor(imageID, startX, startY, dir, depth) { m_hPoints = health; }

	virtual ~ActiveActor() {}

	int getHealth() const { return m_hPoints; }

private:
	int m_hPoints;
};

class PassiveActor : public Actor
{
public:
	PassiveActor(int imageID, double startX, double startY, Direction dir, int depth)
		:Actor(imageID, startX, startY, dir, depth) {}

	virtual ~PassiveActor() {}

	virtual void doSomething() { return; }
};

class Socrates : public ActiveActor
{
public:
	Socrates(StudentWorld* studentWorld)
		:ActiveActor(IID_PLAYER, 0, 128, 0, 0, 100)
	{
		m_flames = 5;
		m_sprays = 20;
		m_studentWorld = studentWorld;
	}

	virtual bool isAlive() const { return getHealth() > 0; }

	virtual ~Socrates() {}

	virtual void doSomething();

private:
	int m_flames;
	int m_sprays;
	StudentWorld* m_studentWorld;
};

class Dirt : public PassiveActor
{
public:
	Dirt(double x, double y)
		:PassiveActor(IID_DIRT, x, y, 0, 1) {}

	virtual ~Dirt() {}
};

void polarToCartesian(Direction angle, double radius, double& dx, double& dy);

#endif // ACTOR_H_
