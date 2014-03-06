#include "FlyPlatform.h"

FlyPlatform::FlyPlatform(float x, float y, int width, int height)
{

}
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
float FlyPlatform::getX()
{
	return x;
}
float FlyPlatform::getY()
{
	return y;
}
int FlyPlatform::getWidth()
{
	return width;
}
int FlyPlatform::getHeight()
{
	return height;
}

