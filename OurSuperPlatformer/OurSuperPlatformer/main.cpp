#include <windows.h>
#include <xinput.h>
#include <winerror.h>
//#include <dinput.h>
#include <iostream>
#include "Player.h"
#include "Surface.h"
#include "FlyPlatform.h"
#include "WinPoint.h"
#include <GL/glut.h>
#include <vector>
#include <math.h>


 // Changed from "Xinput" to retain compatibility with Windows 7 and older machines.
#pragma comment(lib, "Xinput9_1_0.lib")

XINPUT_STATE gamepadState;

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

GLsizei width, height;
Player player;

bool AButton, BButton,XButton,YButton, downPad, upPad,rightPad,leftPad, startButton,backButton, leftSholder, rightSholder;

float leftTrigger = 0;
float rightTrigger = 0;
float leftStickY = 0;
float leftStickX = 0;
float rightStickX = 0;
float rightStickY = 0;
bool flyPlatOn;

bool gamepadConnected = false;

bool jumpKeyPressed;
bool downKey = false, upKey = false, rightKey = false, leftKey = false, cKey = false;

int platformStandingOn = 0;

enum collision {NOTHING, COLLISION};
//enum collision {NOTHING, TOPCOLLISION, BOTTOMCOLLISION, LEFTCOLLISION, RIGHTCOLLISION};

collision checkPlatformCollision(Player player, Surface surface);
collision checkDoorCollision(Player player, WinPoint WinPoint);
//collision checkPlatformCollision(Player player, FlyPlatform fPlatform);
//enum buttFluff {NOTHING, Y, X};
//enum buttFluff {NOTHING, COLLISION};

using namespace std;

//array of surfaces
vector<Surface> surfaces;
vector<FlyPlatform> fPlatforms;

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

static char theStringBuffer[10];
static long font = (long)GLUT_BITMAP_8_BY_13;

void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void floatToString(char * destStr, int precision, int val) 
{
   sprintf(destStr,"%i",val);
   destStr[precision] = '\0';
}

static float gameState = 0;

void loseScreen()
{
   glRasterPos2f(600, 400);
   writeBitmapString((void*)font, "Game Over");
   writeBitmapString((void*)font, theStringBuffer);
}

void winScreen()
{
   glRasterPos2f(600, 400);
   writeBitmapString((void*)font, "Congratulations!");
   writeBitmapString((void*)font, theStringBuffer);
}

	//end point
	WinPoint winner(1100,670,50,50);

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (gameState == 0)
	{
		for (int i = 0; i < surfaces.size(); i++)
			surfaces[i].draw();
		for (int i = 0; i < fPlatforms.size(); i++)
		{
			fPlatforms[i].draw();
		}
		winner.draw();
		player.draw();
	}

	else if (gameState == 1)
	{
		loseScreen();
	}
	else if (gameState == 2)
	{
		winScreen();
	}

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
			cKey = true;
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
		case 99:
			// Set state to FLYING if player is not flying.
			cKey = false;
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
		upKey = upPad;
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

		if (gamepadState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			gamepadState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			leftStickX = 0;
		if (gamepadState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			gamepadState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			leftStickY = 0;
		if (gamepadState.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			gamepadState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			rightStickX = 0; 
		if (gamepadState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			gamepadState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			rightStickY = 0; 
	}
	if (player.getState() == 2)
		player.setV(1*leftStickX,1*leftStickY);
	else
		player.setV(1*leftStickX,0);
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
	if (player.getState() == 0)
	{
			// Set state to JUMPING, this will let gravity take control...
			player.setState(1);
			// ...But not before immediately set the gravity to the beginning of descension, without this the player would jump up automatically. Ocarina of Time this is not.
			player.setGravity(player.gravityAtBeginFall);
	}

	for (int i = 0; i < surfaces.size(); i++)
	{
		if (checkPlatformCollision(player,surfaces[i])==COLLISION)
		{
			
			// When a collision is detected, either one of the two statements below will call depending on whether the player is ascending or descending during a jump.

			// ASCENDING - If the player's state is JUMPING and the current gravity is greater than the rate at which the player begins to fall.
			if (player.getState() == 1 && player.getGravity() > player.gravityAtBeginFall && player.getY() < surfaces[i].getY()-surfaces[i].getHeight()+2)
			{
				// Track the platform being hit. Since this platform is being tracked, the if statement for side collision will not be called for this surface
				// because one of the conditions require the platform not to be this one. 
				platformStandingOn = i;
				// Bounce the player off so he can begin falling
				player.setGravity(player.gravityAtBeginFall-2);
				break;
			}
			// DESCENDING - If the player's state is JUMPING and the player is falling.
			else if (player.getState() == 1 && player.getGravity() < player.gravityAtBeginFall && player.getY() > surfaces[i].getY()+2)
			{
				// Track the platform being hit. Since this platform is being tracked, the if statement for side collision will not be called for this surface
				// because one of the conditions require the platform not to be this one. 
				platformStandingOn = i;
				// land() will change the player's state back to ONGROUND as well as reset the gravity so the player will jump correctly every time without setting himself on fire.
				player.land();
				// Adjust the player's Y position so he does not sink into the floor.
				player.setY(surfaces[i].getY()+player.getHeight());
				
				break;
			}
		}
	}

	if (checkDoorCollision(player, winner)==COLLISION)
	{
		gameState = 2;
	}

	flyPlatOn = false;
	for (int i = 0; i < fPlatforms.size(); i++)
	{
		if (checkPlatformCollision(player,fPlatforms[i])==COLLISION)
		{
			
			
			if (player.getState() == 1 && player.getGravity() > player.gravityAtBeginFall && player.getY() < fPlatforms[i].getY()-fPlatforms[i].getHeight()+2)
			{
				platformStandingOn = i;
				player.setGravity(player.gravityAtBeginFall-2);
				break;
			}
			else if (player.getState() == 1 && player.getGravity() < player.gravityAtBeginFall && player.getY() > fPlatforms[i].getY()+2)
			{
				
				platformStandingOn = i;
				flyPlatOn = true;
				player.land();
				player.setY(fPlatforms[i].getY()+player.getHeight());
				
				break;
			}
		}
	
	}
	if ((flyPlatOn && YButton) ||(flyPlatOn && cKey) )
	{
		flyPlatOn = false;
		player.setY(player.getY()+10);
		player.setState(2);
	}
	
	for (int i = 0;i<surfaces.size();i++)
	{
		// If there is a collision found with a second platform, (one that the player is currently not standing on)
		// A side collision will be assumed.
		if (checkPlatformCollision(player,surfaces[i]) == COLLISION && i!=platformStandingOn)
		{		
			// These statements will keep the player to the side of the platform to prevent a passthrough
			if (player.getX() >= surfaces[i].getX())
			{
				player.setX(surfaces[i].getX()+surfaces[i].getWidth()+1);
			}
			else if (player.getX() <= surfaces[i].getX())
			{
				player.setX(surfaces[i].getX()-player.getWidth()-1);
			}
			break;
		}
	}
	
	for (int i = 0;i<fPlatforms.size();i++)
	{
		// If there is a collision found with a second platform, (one that the player is currently not standing on)
		// A side collision will be assumed.
		if (checkPlatformCollision(player,fPlatforms[i]) == COLLISION && i!=platformStandingOn)
		{		
			// These statements will keep the player to the side of the platform to prevent a passthrough
			if (player.getX() >= fPlatforms[i].getX())
			{
				player.setX(fPlatforms[i].getX()+fPlatforms[i].getWidth()+1);
			}
			else if (player.getX() <= fPlatforms[i].getX())
			{
				player.setX(fPlatforms[i].getX()-player.getWidth()-1);
			}
			break;
		}
	}

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
	//0
	FlyPlatform fPlatform(50,40,450,20);
	fPlatforms.push_back(fPlatform);
	//1
	Surface surface(50,140,500,20);
	surfaces.push_back(surface);
	//2
	surface.newSurface(250,70,100,50);
	surfaces.push_back(surface);
	//3
	surface.newSurface(500,70,100,50);
	surfaces.push_back(surface);
	//4
	surface.newSurface(600,120,100,100);
	surfaces.push_back(surface);
	//5
	surface.newSurface(700,170,100,150);
	surfaces.push_back(surface);
	//6
	surface.newSurface(900,170,100,150);
	surfaces.push_back(surface);
	//7
	fPlatform.newSurface(1100,220,100,50);
	fPlatforms.push_back(fPlatform);
	//8
	surface.newSurface(1200,270,100,50);
	surfaces.push_back(surface);
	//9
	surface.newSurface(1100,350,100,50);
	surfaces.push_back(surface);
	//10
	surface.newSurface(1000,370,50,50);
	surfaces.push_back(surface);
	//11
	surface.newSurface(850,320,50,50);
	surfaces.push_back(surface);
	//12
	surface.newSurface(800,370,50,50);
	surfaces.push_back(surface);
	//13
	surface.newSurface(650,370,50,50);
	surfaces.push_back(surface);
	//14
	surface.newSurface(450,220,50,50);
	surfaces.push_back(surface);
	//15
	surface.newSurface(400,270,50,50);
	surfaces.push_back(surface);
	//16
	surface.newSurface(200,270,50,50);
	surfaces.push_back(surface);
	//17
	fPlatform.newSurface(100,370,50,50);
	fPlatforms.push_back(fPlatform);
	//18
	surface.newSurface(150,420,25,25);
	surfaces.push_back(surface);
	//19
	surface.newSurface(200,420,25,25);
	surfaces.push_back(surface);
	//20
	surface.newSurface(275,470,25,25);
	surfaces.push_back(surface);
	//21
	surface.newSurface(400,395,25,25);
	surfaces.push_back(surface);
	//22
	surface.newSurface(375,520,25,75);
	surfaces.push_back(surface);
	//23
	surface.newSurface(425,570,25,25);
	surfaces.push_back(surface);
	//24
	surface.newSurface(500,565,25,25);
	surfaces.push_back(surface);
	//25
	surface.newSurface(550,595,25,25);
	surfaces.push_back(surface);
	//26
	surface.newSurface(575,520,25,25);
	surfaces.push_back(surface);
	//27
	surface.newSurface(675,555,25,25);
	surfaces.push_back(surface);
	//28
	surface.newSurface(625,595,25,25);
	surfaces.push_back(surface);
	//29
	surface.newSurface(700,620,25,25);
	surfaces.push_back(surface);
	//30
	surface.newSurface(725,645,25,25);
	surfaces.push_back(surface);
	//31
	surface.newSurface(825,615,25,25);
	surfaces.push_back(surface);
	//32
	surface.newSurface(925,645,25,25);
	surfaces.push_back(surface);
	//33
	surface.newSurface(1000,620,300,50);
	surfaces.push_back(surface);
	//walls
	surface.newSurface(0,1000,50,1000);
	surfaces.push_back(surface);
	surface.newSurface(1250,1000,50,1000);
	surfaces.push_back(surface);
	//ceiling
	surface.newSurface(0,750,1250,50);
	surfaces.push_back(surface);
}

collision checkPlatformCollision(Player player, Surface surface)
{	
	if ((player.getX() + player.getWidth()) >= surface.getX() && player.getX() <= (surface.getX() + surface.getWidth()) && player.getY() - player.getHeight() <= surface.getY() && player.getY() >= (surface.getY()-surface.getHeight()))
	{
		return COLLISION;
	}
	return NOTHING;
}

collision checkDoorCollision(Player player, WinPoint surface)
{	
	if ((player.getX() + player.getWidth()) >= surface.getX() && player.getX() <= (surface.getX() + surface.getWidth()) && player.getY() - player.getHeight() <= surface.getY() && player.getY() >= (surface.getY()-surface.getHeight()))
	{
		return COLLISION;
	}
	return NOTHING;
}

/*collision checkPlatformCollision(Player player, FlyPlatform fPlatform)
{	
	if ((player.getX() + player.getWidth()) >= fPlatform.getX() && player.getX() <= (fPlatform.getX() + fPlatform.getWidth()) && player.getY() - player.getHeight() <= fPlatform.getY() && player.getY() >= (fPlatform.getY()-fPlatform.getHeight()))
	{
		return COLLISION;
	}
	return NOTHING;
}*/

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