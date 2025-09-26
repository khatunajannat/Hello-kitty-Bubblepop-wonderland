#ifndef BUBBLESHOOT_H
#define BUBBLESHOOT_H
#include "globals.hpp"
#include <math.h>
#include <vector>
#include <ctime>
#include <queue>
#include <string>
#include<iostream>
#include<fstream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int score = 0;
string playerName;


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


const int BUBBLE_RADIUS = 20;

struct Bubble {
	float x, y;
	int radius;
	bool active;
	bool falling;
	int color;
	float dx, dy;
	bool cinnamon;
	int powerType;
};

vector<Bubble> bubbles;
Bubble currentBubble;
Bubble nextBubble;
float cannonAngle = 90;
int bubbleRadius = 18;
int rows = 5, cols = 15;
bool shooting = false;

int shotsLeft = 30;
bool gameOver = false;
bool gameWon = false;

bool slidingNext = false;
double slideX, slideY;



int colors[5][3] = {
	{ 255, 0, 0 },     // red
	{ 0, 255, 0 },     // green
	{ 186, 85, 211 },     // blue
	{ 255, 255, 0 },   // yellow
};

void playMusic(const char* alias, bool repeat = false) {
	string cmd = string("play ") + alias;
	if (repeat) cmd += " repeat";
	mciSendString(cmd.c_str(), NULL, 0, NULL);
}
void stopMusic(const char* alias) {
	string cmd = string("stop ") + alias;
	mciSendString(cmd.c_str(), NULL, 0, NULL);
	cmd = string("close ") + alias;
	mciSendString(cmd.c_str(), NULL, 0, NULL);
}

void playPopSound() {
	PlaySound(TEXT("pop.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}


static int clampInt(int v, int lo, int hi) {
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}


void snapToGrid(Bubble &b) {
	int spacing = 2;
	int cellSize = 2 * bubbleRadius + spacing;
	int offsetX = (SCREEN_WIDTH - (cols * cellSize)) / 2;

	// estimate column and row
	float colf = (b.x - offsetX - bubbleRadius) / (float)cellSize;
	float rowf = (SCREEN_HEIGHT - b.y - bubbleRadius) / (float)cellSize;

	int col = clampInt((int)round(colf), 0, cols - 1);
	int row = clampInt((int)round(rowf), 0, rows - 1);

	b.x = offsetX + col * cellSize + bubbleRadius;
	b.y = SCREEN_HEIGHT - (row * cellSize + bubbleRadius);
	b.radius = bubbleRadius;
}

void placeAdjacentTo(int otherIdx, Bubble &b) {
	int spacing = 2;
	float dx = b.x - bubbles[otherIdx].x;
	float dy = b.y - bubbles[otherIdx].y;
	float angle = atan2f(dy, dx);
	float step = (float)(2 * bubbleRadius + spacing);

	float candX = bubbles[otherIdx].x + cosf(angle) * step;
	float candY = bubbles[otherIdx].y + sinf(angle) * step;

	b.x = candX;
	b.y = candY;
	b.radius = bubbleRadius;

	snapToGrid(b);
}



void initBubbles() {
	if (gamepages == 4) {
		bubbles.clear();
		int spacing = 2;
		int offsetX = (SCREEN_WIDTH - (cols * (2 * bubbleRadius + spacing))) / 2;
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				Bubble b;
				b.x = offsetX + c * (2 * bubbleRadius + spacing) + bubbleRadius;
				b.y = SCREEN_HEIGHT - (r * (2 * bubbleRadius + spacing) + bubbleRadius);
				b.radius = bubbleRadius;
				b.active = true;
				b.falling = false;
				b.color = rand() % 4;
				b.dx = b.dy = 0;
				b.cinnamon = false;

				if (r == 0 && c == cols / 2) {
					b.cinnamon = true;
					bubbles.push_back(b);
		
					Bubble cyanBubble = b;
					cyanBubble.y -= (2 * bubbleRadius + spacing);
					cyanBubble.color = 4;
					cyanBubble.cinnamon = false;
					bubbles.push_back(cyanBubble);

					continue;
				}
				bubbles.push_back(b);
			}
		}
	}
	else if (gamepages == 5) {
		bubbles.clear();
		int spacing = 2;
		int offsetX = (SCREEN_WIDTH - (cols * (2 * bubbleRadius + spacing))) / 2;
		int rowClearPlaced = 0;
		int bombPlaced = 0;

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				Bubble b;
				b.x = offsetX + c * (2 * bubbleRadius + spacing) + bubbleRadius;
				b.y = SCREEN_HEIGHT - (r * (2 * bubbleRadius + spacing) + bubbleRadius);
				b.radius = bubbleRadius;
				b.active = true;
				b.falling = false;
				b.color = rand() % 4;
				b.dx = b.dy = 0;
				b.cinnamon = false;
				b.powerType = 0;

				if (r == 0 && c == cols / 2) {
					b.cinnamon = true;
					b.powerType = 0;
				}
				else if (r > 0) {
					if (rowClearPlaced == 0 && rand() % 10 == 0) {
						b.powerType = 1;
						rowClearPlaced = 1;
					}
					else if (bombPlaced == 0 && rand() % 10 == 0) {
						b.powerType = 2;
						bombPlaced = 1;
					}
				}
				bubbles.push_back(b);
			}
		}

		if (rowClearPlaced == 0) {
			int idx;
			do {
				idx = rand() % bubbles.size();
			} while (bubbles[idx].cinnamon || bubbles[idx].y >= SCREEN_HEIGHT - (2 * bubbleRadius + spacing));
			bubbles[idx].powerType = 1;
		}

		if (bombPlaced == 0) {
			int idx;
			do {
				idx = rand() % bubbles.size();
			} while (bubbles[idx].cinnamon || bubbles[idx].y >= SCREEN_HEIGHT - (2 * bubbleRadius + spacing) || bubbles[idx].powerType != 0);
			bubbles[idx].powerType = 2;
		}
	}
	else if (gamepages == 6) {
		bubbles.clear();
		int spacing = 2;
		int offsetX = (SCREEN_WIDTH - (cols * (2 * bubbleRadius + spacing))) / 2;
		int rowClearPlaced = 0;
		int bombPlaced = 0;

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				Bubble b;
				b.x = offsetX + c * (2 * bubbleRadius + spacing) + bubbleRadius;
				b.y = SCREEN_HEIGHT - (r * (2 * bubbleRadius + spacing) + bubbleRadius);
				b.radius = bubbleRadius;
				b.active = true;
				b.falling = false;
				b.color = rand() % 4;
				b.dx = b.dy = 0;
				b.cinnamon = false;
				b.powerType = 0;

				if (r == 0 && c == cols / 2) {
					b.cinnamon = true;
				}
				else if (r > 0) {
					if (rowClearPlaced == 0 && rand() % 8 == 0) { // 🔹 Slightly higher chance than Level 2
						b.powerType = 1;
						rowClearPlaced = 1;
					}
					else if (bombPlaced == 0 && rand() % 8 == 0) {
						b.powerType = 2;
						bombPlaced = 1;
					}
				}
				bubbles.push_back(b);
			}
		}

		// Guarantee at least one special bubble
		if (rowClearPlaced == 0) {
			int idx;
			do {
				idx = rand() % bubbles.size();
			} while (bubbles[idx].cinnamon || bubbles[idx].y >= SCREEN_HEIGHT - (2 * bubbleRadius + spacing));
			bubbles[idx].powerType = 1;
		}
		if (bombPlaced == 0) {
			int idx;
			do {
				idx = rand() % bubbles.size();
			} while (bubbles[idx].cinnamon || bubbles[idx].y >= SCREEN_HEIGHT - (2 * bubbleRadius + spacing) || bubbles[idx].powerType != 0);
			bubbles[idx].powerType = 2;
		}
	}
}





Bubble createBubble(float angle, int color = -1) {
		Bubble b;
		// initialize everything to safe defaults
		b.x = SCREEN_WIDTH / 2;
		b.y = 30;
		b.radius = bubbleRadius;
		b.active = true;
		b.falling = false;
		b.cinnamon = false;
		b.powerType = 0;
		b.dx = b.dy = 0.0f;

		// set color (guard against invalid color arg)
		if (color >= 0 && color < 5) b.color = color;
		else b.color = rand() % 4; // keep original behaviour (first 4 colors)

		// velocity computed from angle
		float angleRad = angle * (float)M_PI / 180.0f;
		float speed = 6.0f;
		b.dx = speed * cosf(angleRad);
		b.dy = speed * sinf(angleRad);

		return b;
	}





void drawCannon() {
	int baseX = SCREEN_WIDTH / 2;
	int baseY = 30;
	int barrelLength = 100;
	int barrelWidth = 20;
	float angleRad = cannonAngle * M_PI / 180.0;

	int tipX = baseX + barrelLength * cos(angleRad);
	int tipY = baseY + barrelLength * sin(angleRad);
	int offsetX = (barrelWidth / 2) * -sin(angleRad);
	int offsetY = (barrelWidth / 2) * cos(angleRad);

	double barrelX[4] = { baseX - offsetX, baseX + offsetX, tipX + offsetX, tipX - offsetX };
	double barrelY[4] = { baseY - offsetY, baseY + offsetY, tipY + offsetY, tipY - offsetY };

	// --- Draw cannon barrel ---
	iSetColor(100, 100, 100);
	iFilledPolygon(barrelX, barrelY, 4);

	// --- Draw cannon base ---
	iSetColor(80, 80, 80);
	iFilledCircle(baseX, baseY, 30);

	// --- Current bubble preview inside cannon ---
	// --- Current bubble preview inside cannon base (centered) ---
	// --- Current bubble preview inside cannon base (centered) ---
	if (!shooting && currentBubble.active) {
		int previewRadius = (int)(bubbleRadius * 0.7);  // 70% of full size

		iSetColor(colors[currentBubble.color][0], colors[currentBubble.color][1], colors[currentBubble.color][2]);
		iFilledCircle(baseX, baseY, previewRadius);

		// keep actual bubble aligned & full size
		currentBubble.x = baseX;
		currentBubble.y = baseY;
		currentBubble.radius = bubbleRadius;
	}



	// --- Next bubble (sliding animation or fixed preview) ---
	if (nextBubble.active) {
		int targetX = baseX + 70;
		int targetY = baseY + 20;

		if (slidingNext) {
			// Move gradually toward cannon base
			double speed = 5.0;
			if (fabs(slideX - baseX) > 2 || fabs(slideY - baseY) > 2) {
				double dx = baseX - slideX;
				double dy = baseY - slideY;
				double dist = sqrt(dx * dx + dy * dy);
				slideX += speed * dx / dist;
				slideY += speed * dy / dist;

				iSetColor(colors[nextBubble.color][0], colors[nextBubble.color][1], colors[nextBubble.color][2]);
				iFilledCircle(slideX, slideY, bubbleRadius / 2);
			}
			else {
				// Reached cannon → swap
				currentBubble = nextBubble;
				currentBubble.radius = bubbleRadius;
				currentBubble.x = baseX;
				currentBubble.y = baseY;
				shooting = false;

				nextBubble = createBubble(cannonAngle);
				nextBubble.radius = bubbleRadius;
				nextBubble.active = true;

				slidingNext = false;
			}
		}
		else {
			// Idle preview (fixed spot)
			iSetColor(colors[nextBubble.color][0], colors[nextBubble.color][1], colors[nextBubble.color][2]);
			iFilledCircle(targetX, targetY, bubbleRadius / 2);

			iSetColor(255, 255, 255);
			iText(targetX - 20, targetY + 30, "Next");
		}
	}
}









void drawBubbles() {

	if (gamepages == 4){
		for (auto &b : bubbles) {
			if (b.active) {
				if (b.cinnamon) {
					iShowBMP2(b.x - b.radius, b.y - b.radius, "img//cinnamon.bmp", 0);
				}
				else {
					iSetColor(colors[b.color][0], colors[b.color][1], colors[b.color][2]);
					iFilledCircle(b.x, b.y, b.radius);
				}
			}
		}
		if (currentBubble.active && shooting) {
			iSetColor(colors[currentBubble.color][0], colors[currentBubble.color][1], colors[currentBubble.color][2]);
			iFilledCircle(currentBubble.x, currentBubble.y, currentBubble.radius);
		}
	}

	else if (gamepages == 5){
		for (auto &b : bubbles) {
			if (b.active) {
				if (b.cinnamon) {

					iShowBMP2(b.x - b.radius, b.y - b.radius, "img//cinnamon2.bmp", 0);
				}
				else if (b.powerType == 1) {

					iSetColor(200, 0, 200);
					iFilledCircle(b.x, b.y, b.radius);
					iSetColor(255, 255, 255);
					iText(b.x - 5, b.y - 5, "R");
				}
				else if (b.powerType == 2) {

					iSetColor(0, 200, 200);
					iFilledCircle(b.x, b.y, b.radius);
					iSetColor(255, 255, 255);
					iText(b.x - 5, b.y - 5, "B");
				}
				else {

					iSetColor(colors[b.color][0], colors[b.color][1], colors[b.color][2]);
					iFilledCircle(b.x, b.y, b.radius);
				}
			}
		}


		if (currentBubble.active && shooting) {
			iSetColor(colors[currentBubble.color][0], colors[currentBubble.color][1], colors[currentBubble.color][2]);
			iFilledCircle(currentBubble.x, currentBubble.y, currentBubble.radius);
		}
	
	
	}
	else if (gamepages == 6) {
		for (auto &b : bubbles) {
			if (b.active) {
				if (b.cinnamon) {
					iShowBMP2(b.x - b.radius, b.y - b.radius, "img//cinnamon3.bmp", 0); // 🔹 new cinnamon image
				}
				else if (b.powerType == 1) {
					iSetColor(255, 100, 0); // maybe orange row-clear bubble
					iFilledCircle(b.x, b.y, b.radius);
					iSetColor(255, 255, 255);
					iText(b.x - 5, b.y - 5, "R");
				}
				else if (b.powerType == 2) {
					iSetColor(100, 0, 255); // purple bomb bubble
					iFilledCircle(b.x, b.y, b.radius);
					iSetColor(255, 255, 255);
					iText(b.x - 5, b.y - 5, "B");
				}
				else {
					iSetColor(colors[b.color][0], colors[b.color][1], colors[b.color][2]);
					iFilledCircle(b.x, b.y, b.radius);
				}
			}
		}

		if (currentBubble.active && shooting) {
			iSetColor(colors[currentBubble.color][0], colors[currentBubble.color][1], colors[currentBubble.color][2]);
			iFilledCircle(currentBubble.x, currentBubble.y, currentBubble.radius);
		}
	}

}




int checkCollision(Bubble &b) {
	for (size_t i = 0; i < bubbles.size(); i++) {
		Bubble &other = bubbles[i];
		if (other.active && !other.falling) {
			float dx = b.x - other.x;
			float dy = b.y - other.y;
			float dist = sqrtf(dx*dx + dy*dy);
			if (dist <= b.radius + other.radius) {
				return (int)i; // hit another bubble
			}
		}
	}
	if (b.y + b.radius >= SCREEN_HEIGHT) return -1; // hit ceiling
	return -2; // still flying
}






void popMatches(int idx) {
	// If the hit bubble is cinnamon, player wins immediately
	if (gamepages == 4) {
		if (bubbles[idx].cinnamon) {
			gameWon = true;
			stopMusic("bgsong");
			playMusic("winsong", false);
			return; // exit function, no need to check for matching
		}

		int targetColor = bubbles[idx].color;
		vector<int> visited(bubbles.size(), 0);
		queue<int> q;
		q.push(idx);
		visited[idx] = 1;
		vector<int> group;
		group.push_back(idx);

		while (!q.empty()) {
			int cur = q.front(); q.pop();
			for (size_t i = 0; i < bubbles.size(); i++) {
				if (!visited[i] && bubbles[i].active && !bubbles[i].falling && !bubbles[i].cinnamon && bubbles[i].color == targetColor) {
					float dx = bubbles[cur].x - bubbles[i].x;
					float dy = bubbles[cur].y - bubbles[i].y;
					float dist = sqrt(dx*dx + dy*dy);
					if (dist <= bubbleRadius * 2 + 2) {
						visited[i] = 1;
						q.push(i);
						group.push_back(i);
					}
				}
			}
		}

		if (group.size() >= 3) {
			playMusic("pop", false);
			for (int id : group) {
				bubbles[id].active = false;
				bubbles[id].falling = true;
				bubbles[id].dy = -5;
				bubbles[id].dx = (rand() % 5 - 2);
				score++;
			}
		}
	}
	else if (gamepages == 5 || gamepages == 6) {
		// For levels 2 and 3: cinnamon, powerType 1 (row), powerType 2 (bomb),
		// and normal color-matching groups behave the same.
		if (bubbles[idx].cinnamon) {
			gameWon = true;
			stopMusic("bgsong");
			playMusic("winsong", false);
			return;
		}

		if (bubbles[idx].powerType == 1) {
			float targetY = bubbles[idx].y;
			for (auto &bb : bubbles) {
				if (bb.active && !bb.cinnamon && fabs(bb.y - targetY) < bubbleRadius) {
					bb.active = false;
					bb.falling = true;
					bb.dy = -5;
					bb.dx = (rand() % 5 - 2);
					score++;
				}
			}
			playMusic("pop", false);
			return;
		}

		if (bubbles[idx].powerType == 2) {
			for (auto &bb : bubbles) {
				float dx = bubbles[idx].x - bb.x;
				float dy = bubbles[idx].y - bb.y;
				float dist = sqrt(dx*dx + dy*dy);
				if (bb.active && !bb.cinnamon && dist <= bubbleRadius * 3) {
					bb.active = false;
					bb.falling = true;
					bb.dy = -5;
					bb.dx = (rand() % 5 - 2);
					score++;
				}
			}
			playMusic("pop", false);
			return;
		}

		// Normal color-match group removal (same as level 1)
		int targetColor = bubbles[idx].color;
		vector<int> visited(bubbles.size(), 0);
		queue<int> q;
		q.push(idx);
		visited[idx] = 1;
		vector<int> group;
		group.push_back(idx);

		while (!q.empty()) {
			int cur = q.front(); q.pop();
			for (size_t i = 0; i < bubbles.size(); i++) {
				if (!visited[i] && bubbles[i].active && !bubbles[i].falling && !bubbles[i].cinnamon && bubbles[i].color == targetColor) {
					float dx = bubbles[cur].x - bubbles[i].x;
					float dy = bubbles[cur].y - bubbles[i].y;
					float dist = sqrt(dx*dx + dy*dy);
					if (dist <= bubbleRadius * 2 + 2) {
						visited[i] = 1;
						q.push(i);
						group.push_back(i);
					}
				}
			}
		}

		if (group.size() >= 3) {
			playMusic("pop", false);
			for (int id : group) {
				bubbles[id].active = false;
				bubbles[id].falling = true;
				bubbles[id].dy = -5;
				bubbles[id].dx = (rand() % 5 - 2);
				score++;
			}
		}
	}
}




void removeFloatingBubbles() {
	vector<int> visited(bubbles.size(), 0);
	queue<int> q;
	for (size_t i = 0; i<bubbles.size(); i++) {
		if (bubbles[i].active && !bubbles[i].falling && bubbles[i].y + bubbleRadius >= SCREEN_HEIGHT - 1) {
			visited[i] = 1; q.push(i);
		}
	}
	while (!q.empty()) {
		int cur = q.front(); q.pop();
		for (size_t i = 0; i<bubbles.size(); i++) {
			if (!visited[i] && bubbles[i].active && !bubbles[i].falling) {
				float dx = bubbles[cur].x - bubbles[i].x;
				float dy = bubbles[cur].y - bubbles[i].y;
				float dist = sqrt(dx*dx + dy*dy);
				if (dist <= bubbleRadius * 2 + 2) {
					visited[i] = 1; q.push(i);
				}
			}
		}
	}
	for (size_t i = 0; i<bubbles.size(); i++) {
		if (bubbles[i].active && !visited[i] && !bubbles[i].falling) {
			bubbles[i].falling = true;
			bubbles[i].dy = -4;
			bubbles[i].dx = (rand() % 5 - 2);
		}
	}
}

void fixBubble(Bubble &b) {
	int nearestIdx = -1;
	float nearestDist = 1e9;

	for (size_t i = 0; i < bubbles.size(); i++) {
		Bubble &other = bubbles[i];
		if (other.active && !other.falling && !other.cinnamon && other.powerType == 0) {
			float dx = b.x - other.x;
			float dy = b.y - other.y;
			float dist = sqrtf(dx*dx + dy*dy);
			if (dist < nearestDist) {
				nearestDist = dist;
				nearestIdx = (int)i;
			}
		}
	}

	if (nearestIdx >= 0) {
		float dx = b.x - bubbles[nearestIdx].x;
		float dy = b.y - bubbles[nearestIdx].y;
		float len = sqrtf(dx*dx + dy*dy);
		if (len < 1e-3f) len = 1.0f;
		float step = (float)(b.radius + bubbles[nearestIdx].radius);
		b.x = bubbles[nearestIdx].x + dx / len * step;
		b.y = bubbles[nearestIdx].y + dy / len * step;
	}
	else {
		if (b.y + b.radius >= SCREEN_HEIGHT) {
			b.y = SCREEN_HEIGHT - b.radius;
		}
	}

	b.active = true;
	b.dx = b.dy = 0;
	b.falling = false;
	bubbles.push_back(b);

	int idx = bubbles.size() - 1;
	if (!b.cinnamon) {
		int idx = bubbles.size() - 1;
		popMatches(idx);
	}
	removeFloatingBubbles();
}
void saveScore(const string &playerName) {
	ofstream fout("scores.txt", ios::app);
	fout << playerName << " " << score << endl;
	fout.close();
}

void updateGame() {
	if (gameOver || gameWon) return;

	if (currentBubble.active && shooting) {
		currentBubble.x += currentBubble.dx;
		currentBubble.y += currentBubble.dy;

		if (currentBubble.x <= bubbleRadius || currentBubble.x >= SCREEN_WIDTH - bubbleRadius)
			currentBubble.dx = -currentBubble.dx;

		int colIdx = checkCollision(currentBubble);
		if (colIdx != -2) {
			if (colIdx >= 0 && bubbles[colIdx].cinnamon) {
				gameWon = true;
				stopMusic("bgsong");
				playMusic("winsong", false);
				saveScore(playerName);
				return;
			}

			if (colIdx >= 0 && bubbles[colIdx].powerType != 0) {
				popMatches(colIdx);
				currentBubble.active = false;
				shooting = false;

				currentBubble = nextBubble;
				currentBubble.dx = 0;
				currentBubble.dy = 0;
				currentBubble.x = SCREEN_WIDTH / 2;
				currentBubble.y = 30;

				nextBubble = createBubble(cannonAngle);
				nextBubble.dx = 0;
				nextBubble.dy = 0;
				nextBubble.active = true;
				return;
			}

			fixBubble(currentBubble);

			currentBubble.active = false;
			shooting = false;

			if (--shotsLeft <= 0 && !gameWon) {
				gameOver = true;
				stopMusic("bgsong");
				playMusic("ggsong", false);
				saveScore(playerName);
			}

			currentBubble = nextBubble;
			currentBubble.dx = 0;
			currentBubble.dy = 0;
			currentBubble.x = SCREEN_WIDTH / 2;
			currentBubble.y = 30;

			nextBubble = createBubble(cannonAngle);
			nextBubble.dx = 0;
			nextBubble.dy = 0;
			nextBubble.active = true;
		}
	}

	for (auto &b : bubbles) {
		if (b.active && b.falling) {
			b.x += b.dx;
			b.y += b.dy;
			if (b.y < -BUBBLE_RADIUS) b.active = false;
		}
	}
}
void shoot(){
	if (!shooting && currentBubble.active && !gameOver && !gameWon){
		float speed = 6;
		float angleRad = cannonAngle*M_PI / 180.0;
		currentBubble.dx = speed*cos(angleRad);
		currentBubble.dy = speed*sin(angleRad);
		shooting = true;
	}
}


void resetLevel() {
	bubbles.clear();

	currentBubble = Bubble();
	nextBubble = Bubble();
	shooting = false;
	slidingNext = false;

	// Reset flags
	gameOver = false;
	gameWon = false;

	shotsLeft = 30; // reset ammo

	// Reinitialize bubbles for the current level
	initBubbles();

	// Initialize current + next bubbles
	currentBubble = createBubble(cannonAngle);
	currentBubble.dx = 0;
	currentBubble.dy = 0;
	currentBubble.active = true;
	currentBubble.radius = bubbleRadius;

	nextBubble = createBubble(cannonAngle);
	nextBubble.dx = 0;
	nextBubble.dy = 0;
	nextBubble.active = true;
}



#endif
