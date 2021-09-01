#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Socrates		
void Socrates::doSomething()
{
	int ch;
	double x, y;
	if (m_studentWorld->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			polarToCartesian(getDirection() + 185, VIEW_RADIUS, x, y);
			moveTo(VIEW_WIDTH / 2 + x, VIEW_HEIGHT / 2 + y);
			setDirection((getDirection() + 5));
			break;
		case KEY_PRESS_RIGHT:
			polarToCartesian(getDirection() + 175, VIEW_RADIUS, x, y);
			moveTo(VIEW_WIDTH / 2 + x, VIEW_HEIGHT / 2 + y);
			setDirection((getDirection() - 5));
			break;

		}
	}
}

void polarToCartesian(Direction angle, double radius, double& dx, double& dy)
{
	const double PI = 4 * atan(1);
	dx = radius * cos(angle * 1.0 / 360 * 2 * PI);
	dy = radius * sin(angle * 1.0 / 360 * 2 * PI);
}
