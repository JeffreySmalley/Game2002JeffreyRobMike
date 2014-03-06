#ifndef SURFACE_H
#define SURFACE_H
#include <GL/glut.h>
class Surface
{
protected:
	float x, y;
	int width, height;
public:
	Surface(float x, float y, int width, int height);
	void draw();
	
	float getX();
	float getY();

	float getWidth();
	float getHeight();
	void newSurface(float x, float y, int width, int height);
};
#endif