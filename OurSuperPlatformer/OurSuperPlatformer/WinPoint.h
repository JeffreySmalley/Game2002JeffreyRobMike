#include <GL/glut.h>
class WinPoint
{
private:
	float x, y;
	int width, height;
public:
	WinPoint(float x, float y, int width, int height);
	void draw();
	
	float getX();
	float getY();

	float getWidth();
	float getHeight();
	void newWinPoint(float x, float y, int width, int height);
};