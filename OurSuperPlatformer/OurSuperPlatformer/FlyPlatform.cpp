#include "FlyPlatform.h"


void FlyPlatform::draw()
{
	glColor3f(0.0,0.5,0.5);
	glBegin(GL_QUADS);

	glVertex3f(x,y,0.0);
	glVertex3f(x+width,y,0.0);
	glVertex3f(x+width,y-height,0.0);
	glVertex3f(x,y-height,0.0);
	glEnd();
}

