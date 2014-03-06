#include <GL/glut.h>
#include "Surface.h"
class WinPoint: public Surface
{
public:
	WinPoint(float x, float y, int width, int height):Surface(x,y,width,height){};
	void draw();
};