#include "Surface.h"

Surface::Surface(float x, float y, int width, int height)
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
void Surface::draw()
{
	glColor3f(0.0,0.0,0.5);
	glBegin(GL_QUADS);
	//glVertex3f(x,y,0.0);
	//glVertex3f(x+width,y,0.0);
	//glVertex3f(x+width,y+height,0.0);
	//glVertex3f(x,y+height,0.0);

	glVertex3f(x,y,0.0);
	glVertex3f(x+width,y,0.0);
	glVertex3f(x+width,y-height,0.0);
	glVertex3f(x,y-height,0.0);
	glEnd();
}
float Surface::getX()
{
	return x;
}
float Surface::getY()
{
	return y;
}
float Surface::getHeight()
{
	return height;
}
float Surface::getWidth()
{
	return width;
}
void Surface::newSurface(float x, float y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}