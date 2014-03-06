#include <GL/glut.h>
#include "Surface.h"

class FlyPlatform: public Surface
{
public:
	FlyPlatform(float x, float y, int width, int height):Surface(x,y,width,height){};
	void draw();
};