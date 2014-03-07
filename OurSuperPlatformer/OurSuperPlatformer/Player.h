#include <gl/glut.h>
#include "Surface.h"

class Player
{	
	
public:
	// The gravity rate at which the player begins to fall is stored in this float.
	float gravityAtBeginFall;
	// hasLanded will likely become obsolete, no point in using it if getState() does just fine.
	// However in earlier builds the player's state would rapidly change between ONGROUND and JUMPING
	// So this is being kept just in case...
	bool hasLanded;
	// ...Same goes for these variables.
	bool movingUp, movingDown, movingLeft, movingRight;
	
	int platformStandingOn;
	
	Player();
	
	void draw();
	void update();
	void jump();
	void land();
	bool die();
	float getX();
	float getY();
	float getWidth();
	float getHeight();
	int getState();
	void setState(int state);
	void setGravity(float gravity);
	float getGravity();
	void setV(float x, float y);
	void setY(float y);
	void setX(float x);

	bool movePlayer();

private:
	struct velocity
	{
		float x;
		float y;
	};
	
	int deathCounter;

	float colorR, colorG, colorB;

	float maxVX;
	float slowdown;
	
	velocity v;

	float gravity;

	float x, y;
	
	bool run;
	float fallSpeed;
	float width, height;
	
	enum stateList{ONGROUND,JUMPING,FLYING,DYING};
	stateList state;
	
};