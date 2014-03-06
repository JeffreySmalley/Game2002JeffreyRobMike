#include "WinPoint.h"
void WinPoint::draw()
{
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
	glVertex3f(x,y,0.0);
	glVertex3f(x+width,y,0.0);
	glVertex3f(x+width,y-height,0.0);
	glVertex3f(x,y-height,0.0);
	glEnd();
}