#include "WinPoint.h"

WinPoint::WinPoint(float x, float y, int width, int height)
{
	//x = 50;
	//y = 100;
	//width = 500;
	//height = 20;

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
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
float WinPoint::getX()
{
	return x;
}
float WinPoint::getY()
{
	return y;
}
float WinPoint::getHeight()
{
	return height;
}
float WinPoint::getWidth()
{
	return width;
}
void WinPoint::newWinPoint(float x, float y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}