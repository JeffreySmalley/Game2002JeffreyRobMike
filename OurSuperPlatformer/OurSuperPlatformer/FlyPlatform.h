#include <GL/glut.h>

class FlyPlatform
{
private:
	float x, y;
	int width, height;

public:
	FlyPlatform(float x, float y, int width, int height);
	void draw();
	float getX();
	float getY();
	int getWidth();
	int getHeight();
	void newPlatform(float x, float y, int width, int height);
};