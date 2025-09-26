#define _CRT_SECURE_NO_WARNINGS
#include "iGraphics.h"
#include "menu.hpp"
#include "bubbleShoot.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


#include "globals.hpp"

int gamepages = 0;
int hover = 0;
int story = 0;

void iDraw(){

	iClear();
	menu();
	menuChange();

	if (gamepages == 4){
		iShowBMP(0, 0, "img//bg.bmp");
		drawCannon();
		drawBubbles();
		iSetColor(255, 255, 255);
		char msg[50];
		sprintf_s(msg, sizeof(msg), "Shots Left: %d", shotsLeft);
		iText(20, 20, msg);
		if (gameOver) {
			iShowBMP2(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, "img//gameover.bmp", 0);
		}
		if (gameWon) {
			iShowBMP2(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, "img//victory.bmp", 0);
		}

	}

	else if (gamepages == 5){

		iShowBMP(0, 0, "img//bg2.bmp");
		drawCannon();
		drawBubbles();
		iSetColor(255, 255, 255);
		char msg[50];
		sprintf_s(msg, sizeof(msg), "Shots Left: %d", shotsLeft);
		iText(20, 20, msg);
		if (gameOver) {
			iShowBMP2(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, "img//gameover.bmp", 0);
		}
		if (gameWon) {
			iShowBMP2(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, "img//victory2.bmp", 0);
		}

	}
	
	

	else if (gamepages == 6) {
		iShowBMP(0, 0, "img//bg3.bmp");  // 🔹 use new background
		drawCannon();
		drawBubbles();
		iSetColor(255, 255, 255);
		char msg[50];
		sprintf_s(msg, sizeof(msg), "Shots Left: %d", shotsLeft);
		iText(20, 20, msg);

		if (gameOver) {
			iShowBMP2(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, "img//gameover.bmp", 0);
		}
		if (gameWon) {
			iShowBMP2(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, "img//victory3.bmp", 0); // 🔹 new victory image if you want
		}
	}




}

void iMouseMove(int mx, int my)
{
	float dx = mx - SCREEN_WIDTH / 2;
	float dy = my - 30;
	float angle = atan2(dy, dx)*180.0 / M_PI;
	if (angle<10) angle = 10;
	if (angle>170) angle = 170;
	cannonAngle = angle;


}

void iPassiveMouseMove(int mx, int my)
{
	mouseHover(mx, my);
	iMouseMove(mx, my);
	
}

void iMouse(int button, int state, int mx, int my)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("%d %d \n", mx, my);
		mouseClick(mx, my);

		if (justEnteredLevel) {
			// Ignore this click so cannon doesn't fire immediately
			justEnteredLevel = false;
			return;
		}

		if ((gamepages == 4 || gamepages == 5 || gamepages == 6)
			&& currentBubble.active && !shooting) {
			shoot();
		}
	}

}

// Special Keys:
// GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12, 
// GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP, GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT

void fixedUpdate()
{

	if (isKeyPressed('q')&& gamepages >1){

		if (gamepages == 4 || gamepages == 5 || gamepages == 6) {
			resetLevel();  
		}
		gamepages = 1;      
	}



	if (isKeyPressed('w') || isSpecialKeyPressed(GLUT_KEY_UP))
	{
		//y++;
	}
	if (isKeyPressed('a') || isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		//x--;
	}
	if (isKeyPressed('s') || isSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		//y--;
	}
	if (isKeyPressed('d') || isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		//x++;
	}

	if (isKeyPressed(' ')) {
		// Playing the audio once
		//mciSendString("play ggsong from 0", NULL, 0, NULL);
	}
}
void timer(){ updateGame(); }



int main()
{
	
	cout << "Enter your name: ";
	cin >> playerName;
	saveScore(playerName);
	srand(time(0));
	initBubbles();
	currentBubble = createBubble(cannonAngle);
	currentBubble.dx = 0;
	currentBubble.dy = 0;
	currentBubble.active = true;
	currentBubble.radius = bubbleRadius;
	shooting = false;

	nextBubble = createBubble(cannonAngle);
	nextBubble.dx = 0;
	nextBubble.dy = 0;
	nextBubble.active = true;
	nextBubble.radius = bubbleRadius;


	mciSendString("open \"Audios//background.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//gameover.mp3\" alias ggsong", NULL, 0, NULL);
	mciSendString("open \"Audios//victory.mp3\" alias winsong", NULL, 0, NULL);
	mciSendString("open \"Audios//pop.mp3\" alias pop", NULL, 0, NULL);

	playMusic("bgsong", true);

	iSetTimer(10, timer);
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Bubble Shooter");
	iStart();
	return 0;


}