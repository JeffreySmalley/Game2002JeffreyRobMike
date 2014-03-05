#include <GL/glut.h>

class FlyPlatform
{
private:
	float x, y;
	int width, height;

public:
	FlyPlatform();
	void draw();
	float getX();
	float getY();
	int getWidth();
	int getHeight();
};