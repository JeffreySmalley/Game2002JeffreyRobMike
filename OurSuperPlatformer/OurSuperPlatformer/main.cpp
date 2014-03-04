#include <windows.h>
#include <xinput.h>
#include <winerror.h>
//#include <dinput.h>
#include <iostream>
#include "Player.h"
#include "Surface.h"
#include <GL/glut.h>
#include <vector>
#include <math.h>

#pragma comment(lib, "Xinput.lib")

XINPUT_STATE gamepadState;

#define DeadZone = 0.05f

GLsizei width, height;
Player player;

bool AButton, BButton,XButton,YButton, downPad, upPad,rightPad,leftPad, startButton,backButton, leftSholder, rightSholder;

float leftTrigger = 0;
float rightTrigger = 0;
float leftStickY = 0;
float leftStickX = 0;
float rightStickX = 0;
float rightStickY = 0;

bool gamepadConnected = false;

bool jumpKeyPressed;
bool downKey = false, upKey = false, rightKey = false, leftKey = false;

enum collision {NOTHING, COLLISION};
//enum collision {NOTHING, TOPCOLLISION, BOTTOMCOLLISION, LEFTCOLLISION, RIGHTCOLLISION};

collision checkPlatformCollision(Player player, Surface surface);
//enum buttFluff {NOTHING, Y, X};
//enum buttFluff {NOTHING, COLLISION};

using namespace std;

//array of surfaces
vector<Surface> surfaces;

void setup()
{
	glClearColor(0.0,0.0,0.0,0.0);
}
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
   
	glOrtho(0, 1280, 0, 720, -1, 1);

	glMatrixMode(GL_MODELVIEW);

	width = w;
	height = h;
}
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < surfaces.size(); i++)
		surfaces[i].draw();
	player.draw();

	glutPostRedisplay();
	glutSwapBuffers();
}
void keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
		// X
		case 120:
			jumpKeyPressed = true;
			break;
		case 122:
			//player.run = true;
			break;
		//C
		case 99:
			// Set state to FLYING if player is not flying.
			if (player.getState() != 2)
			{
				player.setState(2);
			}
			else
			{
				// Set state to JUMPING. In midair, a JUMPING state will let the player fall until hitting a platform.
				player.setState(1);
				// Setting gravity to -4.0 will let the player fall immediately instead of jumping up when the state is set to JUMPING.
				player.setGravity(player.gravityAtBeginFall);
			}
			break;
	}
}
void keyRelease(unsigned char key, int x, int y)
{
	switch (key)
	{
		// Z	
		
		case 122:
			//player.run = false;
			break;
		// X
		case 120:
			if (jumpKeyPressed)	
			{
				jumpKeyPressed = false;
				if (player.getState() == 1)
				{
					/*if (player.fallSpeed < 3.5)
					{
						player.fallSpeed = 3.5;
					}*/
				}
			break;
		
		case 15:
			
			break;
		
		}
	}
}
void specialKeyPress(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		rightKey = true;
		player.movingRight = true;
		break;
	case GLUT_KEY_LEFT:
		leftKey = true;
		break;
	case GLUT_KEY_UP:
		upKey = true;
		break;
	case GLUT_KEY_DOWN:
		downKey = true;
		break;
	}
}
void specialKeyRelease(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		rightKey = false;
		break;
	case GLUT_KEY_LEFT:
		leftKey = false;
		break;
		case GLUT_KEY_UP:
		upKey = false;
		break;
	case GLUT_KEY_DOWN:
		downKey = false;
		break;
	}
}
void update()
{	
	SecureZeroMemory(&gamepadState,sizeof(XINPUT_STATE));
	if (XInputGetState(0, &gamepadState) == ERROR_SUCCESS)
	{
		gamepadConnected = true;
		AButton = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_A)!= 0);
		BButton = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_B)!= 0);
		YButton = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)!= 0);
		XButton = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_X)!= 0);
		
		upPad = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)!= 0);
		downPad = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)!= 0);
		leftPad = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)!= 0);
		rightPad = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)!= 0);

		leftSholder = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)!=0);
		rightSholder = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)!=0);

		startButton = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_START) !=0);
		backButton = ((gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) !=0);

		leftTrigger = (float)gamepadState.Gamepad.bLeftTrigger /255;
		rightTrigger = (float)gamepadState.Gamepad.bRightTrigger /255;

		leftStickX = max(-1,(float)gamepadState.Gamepad.sThumbLX / 32767);
		leftStickY = max(-1, (float) gamepadState.Gamepad.sThumbLY / 32767);
		rightStickX = max(-1,(float)gamepadState.Gamepad.sThumbLX / 32767);
		rightStickY = max(-1, (float) gamepadState.Gamepad.sThumbLY / 32767);
	}
	else
	{
		if (gamepadConnected)
		{
			AButton = upPad = downPad= rightPad =leftPad= BButton=YButton=XButton= rightSholder= leftSholder = startButton =backButton= false;
			leftTrigger = rightTrigger = leftStickX = leftStickY= rightStickX= rightStickY = 0;
		}
		gamepadConnected = false;
	}

	if (gamepadConnected)
	{
		jumpKeyPressed = AButton;
		leftKey = leftPad;
		rightKey = rightPad;
		upKey = rightPad;
		downKey = downPad;
		if (leftStickX > 1)
			leftStickX = 1;
		if (leftStickX < -1)
			leftStickX = -1;

		if (leftStickY > 1)
			leftStickY = 1;
		if (leftStickY < -1)
			leftStickY = -1;

		if (rightStickX > 1)
			rightStickX = 1;
		if (rightStickX < -1)
			rightStickX = -1;

		if (rightStickY > 1)
			rightStickY = 1;
		if (rightStickY < -1)
			rightStickY = -1;
	}

	if (jumpKeyPressed)
	{
		if (player.getState() != 1 && player.getState() != 2)
		{
			player.setState(1);
		}
	}

	// Controls all actions related to jumping, including the adjustment of gravity while midair.
	player.jump();
	// For now, only the player's position is updated in the update().
	player.update();
	
	for (int i = 0; i < surfaces.size(); i++)
	{
		if (checkPlatformCollision(player,surfaces[i])==COLLISION)
		{
			
			// When a collision is detected, either one of the two statements below will call depending on whether the player is ascending or descending during a jump.

			// ASCENDING - If the player's state is JUMPING and the current gravity is greater than the rate at which the player begins to fall.
			if (player.getState() == 1 && player.getGravity() > player.gravityAtBeginFall)
			{
				player.setGravity(player.gravityAtBeginFall-2);
				player.platformStandingOn = i;
				break;
			}
			// DESCENDING - If the player's state is JUMPING and the player is falling.
			else if (player.getState() == 1 && player.getGravity() < player.gravityAtBeginFall)
			{
				// land() will change the player's state back to ONGROUND as well as reset the gravity so the player will jump correctly every time without setting himself on fire.
				player.land();
				// Adjust the player's Y position so he does not sink into the floor.
				player.setY(surfaces[i].getY()+player.getHeight());
				player.platformStandingOn = i;
				break;
			}
		}
		
		// This else occurs when no collisions have been detected.
		else
		{
			// The following if statement will let the player walk off the edge, poor guy.

			// If the player is ONGROUND
			if (player.getState() == 0)
			{
				// Set state to JUMPING, this will let gravity take control...
				player.setState(1);
				// ...But not before immediately set the gravity to the beginning of descension, without this the player would jump up automatically. Ocarina of Time this is not.
				player.setGravity(player.gravityAtBeginFall);
			}
			//break;
		}
	}
	//if (player.movingRight)
	//{
		for (int i = 0;i<surfaces.size();i++)
		{
			if (checkPlatformCollision(player,surfaces[i]) == COLLISION)
			{
				if (i!=player.platformStandingOn)
				{
					if (player.getX() <= surfaces[i].getX())
					{
						player.setX(surfaces[i].getX()-player.getWidth());
					}
					else if (player.getX() >= surfaces[i].getX())
					{
						player.setX(surfaces[i].getX()+surfaces[i].getWidth());
					}
					if (!rightKey)
					{
						player.setV(-0.1,0);
					}
					break;
				}
			}
		}
	//}
	if (rightKey)
	{
		player.setV(0.1,0);
	}
	else if (leftKey)
	{
		player.setV(-0.1,0);
	}
	else
	{
		player.setV(-800,0);
	}

	

	// If the player's state is FLYING
	if (player.getState()==2)
	{
		if (upKey)
		{
			player.setV(0,0.1);
		}
		else if (downKey)
		{
			player.setV(0,-0.1);
		}
		else
		{
			player.setV(0,-800);
		}
	}
}
void timer(int millisec)
{
	update();
	glutTimerFunc(millisec,timer,millisec);
}

void createLevel()
{	
	Surface surface(50,120,500,20);
	surfaces.push_back(surface);
	surface.newSurface(50,220,500,20);
	surfaces.push_back(surface);
	surface.newSurface(250,150,100,50);
	surfaces.push_back(surface);
	surface.newSurface(500,150,100,50);
	surfaces.push_back(surface);
	surface.newSurface(600,200,100,100);
	surfaces.push_back(surface);
	surface.newSurface(700,250,100,150);
	surfaces.push_back(surface);
	surface.newSurface(900,250,100,150);
	surfaces.push_back(surface);
	surface.newSurface(1100,300,100,50);
	surfaces.push_back(surface);
	surface.newSurface(1200,350,100,50);
	surfaces.push_back(surface);
	surface.newSurface(1100,450,100,50);
	surfaces.push_back(surface);
	surface.newSurface(1000,500,50,50);
	surfaces.push_back(surface);
	surface.newSurface(850,400,50,50);
	surfaces.push_back(surface);
	surface.newSurface(800,450,50,50);
	surfaces.push_back(surface);
	surface.newSurface(650,450,50,50);
	surfaces.push_back(surface);
	surface.newSurface(450,300,50,50);
	surfaces.push_back(surface);
	surface.newSurface(400,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(200,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(100,450,50,50);
	surfaces.push_back(surface);
	surface.newSurface(150,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(200,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(275,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(400,475,25,25);
	surfaces.push_back(surface);
	surface.newSurface(375,550,25,75);
	surfaces.push_back(surface);
	surface.newSurface(425,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(500,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(550,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(575,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(675,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(625,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(700,600,25,25);
	surfaces.push_back(surface);
	surface.newSurface(725,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(825,650,25,25);
	surfaces.push_back(surface);
	surface.newSurface(925,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(1000,600,300,50);
	surfaces.push_back(surface);
	surface.newSurface(450,300,50,50);
	surfaces.push_back(surface);
	surface.newSurface(400,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(200,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(100,450,50,50);
	surfaces.push_back(surface);
	surface.newSurface(150,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(200,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(275,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(400,475,25,25);
	surfaces.push_back(surface);
	surface.newSurface(375,550,25,75);
	surfaces.push_back(surface);
	surface.newSurface(425,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(500,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(550,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(575,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(675,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(625,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(700,600,25,25);
	surfaces.push_back(surface);
	surface.newSurface(725,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(825,650,25,25);
	surfaces.push_back(surface);
	surface.newSurface(925,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(1000,600,300,50);
	surfaces.push_back(surface);
	surface.newSurface(450,300,50,50);
	surfaces.push_back(surface);
	surface.newSurface(400,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(200,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(100,450,50,50);
	surfaces.push_back(surface);
	surface.newSurface(150,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(200,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(275,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(400,475,25,25);
	surfaces.push_back(surface);
	surface.newSurface(375,550,25,75);
	surfaces.push_back(surface);
	surface.newSurface(425,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(500,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(550,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(575,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(675,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(625,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(700,600,25,25);
	surfaces.push_back(surface);
	surface.newSurface(725,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(825,650,25,25);
	surfaces.push_back(surface);
	surface.newSurface(925,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(1000,600,300,50);
	surfaces.push_back(surface);
	surface.newSurface(450,300,50,50);
	surfaces.push_back(surface);
	surface.newSurface(400,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(200,350,50,50);
	surfaces.push_back(surface);
	surface.newSurface(100,450,50,50);
	surfaces.push_back(surface);
	surface.newSurface(150,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(200,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(275,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(400,475,25,25);
	surfaces.push_back(surface);
	surface.newSurface(375,550,25,75);
	surfaces.push_back(surface);
	surface.newSurface(425,550,25,25);
	surfaces.push_back(surface);
	surface.newSurface(500,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(550,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(575,500,25,25);
	surfaces.push_back(surface);
	surface.newSurface(675,525,25,25);
	surfaces.push_back(surface);
	surface.newSurface(625,575,25,25);
	surfaces.push_back(surface);
	surface.newSurface(700,600,25,25);
	surfaces.push_back(surface);
	surface.newSurface(725,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(825,650,25,25);
	surfaces.push_back(surface);
	surface.newSurface(925,625,25,25);
	surfaces.push_back(surface);
	surface.newSurface(1000,600,300,50);
	surfaces.push_back(surface);
}

//collision checkPlatformCollision(Player player, Surface surface)
//{	
	//if ((player.getY() - player.getHeight()) <= surface.getY())
	//{
		//return TOPCOLLISION;
	//}
//}

collision checkPlatformCollision(Player player, Surface surface)
{	
	if ((player.getX() + player.getWidth()) >= surface.getX() && player.getX() <= (surface.getX() + surface.getWidth()) && player.getY() - player.getHeight() <= surface.getY() && player.getY() >= (surface.getY()-surface.getHeight()))
	{
		return COLLISION;
	}
	return NOTHING;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1280,1024);
	glutInitWindowPosition(100,50);

	glutCreateWindow("Go Go!! Tukashi Miyzukipapya Shundiji Genghis Khan!!");

	setup();
	createLevel();
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutSpecialFunc(specialKeyPress);
	glutSpecialUpFunc(specialKeyRelease);
	glutReshapeFunc(resize);
	glutTimerFunc(16,timer,16);
	glutDisplayFunc(draw);

	glutMainLoop();
}