#include <iostream>
#include "Player.h"

using namespace std;

Player::Player()
{
	x = 60;
	y = 60;

	//x = 220;
	//y = 350;

	width = 20;
	height = 20;

	v.x = 0;
	v.y = 0;

	gravity = -4.0;

	maxVX = 3;
	slowdown = 0.5;

	gravityAtBeginFall = -6.0;

	deathCounter = 50;
	
	// The player will spawn a tiny bit above the playing field.
	// State is set to JUMPING to let the player fall into place.
	// The gravity will be set to the rate at which the player begins to fall, without setting this the player would jump at the beginning of the game.
	gravity = gravityAtBeginFall;
	setState(1);
	// 0 = ONGROUND
	// 1 = JUMPING
	// 2 = FLYING
}
void Player::draw()
{
	glPushMatrix();
	glColor3f(colorR,colorG,colorB);

	if (getState() == 0 || getState() == 1)
	{
		colorR = 0.0;
		colorG = 1.0;
		colorB = 0.0;

		//glColor3f(0.0,1.0,0.0);
	}
	else if (getState() == 2)
	{
		colorR = 0.0;
		colorG = 0.5;
		colorB = 0.5;

		//glColor3f(0.0,0.5,0.5);
	}
	else if (getState() == 3)
	{
	  //colorR will be set elsewhere, it will start at 1.0 and rapidly decrease to give a fadeout effect.
		colorG = 0.0;
		colorB = 0.0;
	}
	//glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
	
	glVertex3f(x,y,0.0);
	glVertex3f(x+width,y,0.0);
	glVertex3f(x+width,y-height,0.0);
	glVertex3f(x,y-height,0.0);

	glEnd();
	glPopMatrix();
}
void Player::update()
{
	x += v.x;
	y += v.y;

	if (y < 40)
	{
		setState(3);
	}
}
void Player::jump()
{
	// If player is jumping
	if (getState() == JUMPING)
	{
		hasLanded = false;
		setV(0,2.0);
		this->y += v.y;
		v.y += gravity;
		gravity-=0.05;
	}
}
void Player::land()
{
	state = ONGROUND;
	// Reset gravity so subsequent jumps aren't stupid
	gravity = -1.0;
	hasLanded = true;
}
void Player::setV(float x, float y)
{
	float temp = x;
	if (temp != -800)
	{
		v.x += x;
	}
	
	if (v.x >= maxVX)
	{
		v.x = maxVX;
	}
	if (v.x <= -maxVX)
	{
		v.x = -maxVX;
	}
	if (temp <= -800)
	{
		
		if (v.x<=maxVX && v.x > 0)
		{
			if (v.x - slowdown >= 0)
			{
				v.x -= slowdown;
			}
			else
			{
				v.x = 0;
			}
		}
		else if (v.x >= -maxVX && v.x < 0)
		{
			if (v.x + slowdown <= 0)
			{
				v.x += slowdown;
			}
			else
			{
				v.x = 0;
			}
		}
	}
	
	if (getState()!=FLYING)
	{
		v.y = y;
	}
	else
	{
		float temp = y;
		if (temp != -800)
		{
			v.y += y;
		}
	
	
		if (v.y >= maxVX)
		{
			v.y = maxVX;
		}
		if (v.y <= -maxVX)
		{
			v.y = -maxVX;
		}
		if (temp <= -800)
		{
			if (v.y<=maxVX && v.y > 0)
			{
				if (v.y - slowdown >= 0)
				{
					v.y -= slowdown;
				}
				else
				{
					v.y = 0;
				}
			}
			else if (v.y >= -maxVX && v.y < 0)
			{
				if (v.y + slowdown <= 0)
				{
					v.y += slowdown;
				}
				else
				{
					v.y = 0;
				}
			}
		}
	}
}
bool Player::die()
{
	if (deathCounter == 50)
	{
		colorR = 1.0;
	}
	
	deathCounter--;
	colorR -= 0.02;

	if (deathCounter==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Player::getX()
{
	return x;
}
float Player::getY()
{
	return y;
}
float Player::getHeight()
{
	return height;
}
float Player::getWidth()
{
	return width;
}
int Player::getState()
{
	// If state is equal to ONGROUND
	if (state == 0)
	{
		return ONGROUND;
	}
	// If state is equal to JUMPING
	if (state == 1)
	{
		return JUMPING;
	}
	// If state is equal to FLYING
	if (state == 2)
	{
		return FLYING;
	}
	if (state == 3)
	{
		return DYING;
	}
	return 0;
}
void Player::setState(int state)
{
	if (state == 0)
	{
		this->state = ONGROUND;
	}
	if (state == 1)
	{
		this->state = JUMPING;
	}
	if (state == 2)
	{
		this->state = FLYING;
	} 
	if (state == 3)
	{
		this->state = DYING;
	}
}
void Player::setGravity(float gravity)
{
	this->gravity = gravity;
}
float Player::getGravity()
{
	return gravity;
}
void Player::setY(float y)
{
	this->y = y;
}
void Player::setX(float x)
{
	this->x = x;
}