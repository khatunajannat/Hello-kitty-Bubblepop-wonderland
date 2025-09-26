#ifndef MENU_H
#define MENU_H
#include "globals.hpp"
#include "bubbleShoot.hpp"


void menu(){


if (gamepages == 0) {
		iShowBMP(0, 0, "img\\starting.bmp");
	}
	else if (gamepages == 1) {
		iShowBMP(0, 0, "img\\menu.bmp");
	}
	else if (gamepages == 2) { 
		if (story == 1) {
			iShowBMP(0, 0, "img\\s1.bmp");
		}
		else if (story == 2) {
			iShowBMP(0, 0, "img\\s2.bmp");
		}
		else if (story == 3) {
			iShowBMP(0, 0, "img\\s3.bmp");
		}
		else if (story == 4) {
			iShowBMP(0, 0, "img\\s4.bmp");
		}
		else if (story == 5) {
			iShowBMP(0, 0, "img\\s5.bmp");
		}
		else if (story == 6) {
			iShowBMP(0, 0, "img\\s6.bmp");
		}
	}

	else if(gamepages == 3 ){
	
		iShowBMP(0,0,"img\\lvl.bmp");
	}
	else if (gamepages == 7){
		iShowBMP(0, 0, "img\\leaderboard.bmp");
	}
}


void mouseClick(int mx, int my) {

	if (gamepages == 0) {
		gamepages = 1;
	}
	else if (gamepages == 1) {
		if (mx >= 615 && mx <= 740 && my >= 419 && my <= 472) {
			gamepages = 2;
			story = 1;
		}
		else if (mx >= 600 && mx <= 734 && my >= 330 && my <= 385){
			gamepages = 7;
			story = 1;
		}
		else if (mx >= 625 && mx <= 720 && my >= 250 && my <= 310){
			exit(0);
		}
	}
	else if (gamepages == 2) {
		if (story < 6) {
			story++;
		}
		else {
			gamepages = 3;  // move to level selection page
		}
	}
	else if (gamepages == 3) { // level selection page
		if (mx >= 644 && mx <= 665 && my >= 148 && my <= 183) {
			gamepages = 4;  // Level 1
			resetLevel();
			justEnteredLevel = true;
		}
		else if (mx >= 700 && mx <= 725 && my >= 340 && my <= 400) {
			gamepages = 5;  // Level 2
			resetLevel();
			justEnteredLevel = true;
		}
		else if (mx >= 762 && mx <= 800 && my >= 590 && my <= 640) {
			gamepages = 6;  // Level 3 (NEW)
			resetLevel();
			justEnteredLevel = true;
		}
	}



	}





void mouseHover(int mx, int my){


	
		 if (gamepages == 1) { 
			
			if (mx >= 615 && mx <= 740 && my >= 419 && my <= 472) {
		 
				hover = 1;     
			}
		
			
			else if (mx >=600 && mx<=734 && my >=330 && my<=385){
			
			hover = 2;
			
			}
			
			else if(mx >=625 && mx<= 720 && my >=250 && my<= 310 ){
			
				hover = 3;
			
			}
			else 
				hover = 0;



		}
		
		
}

void menuChange(){

    if (hover == 1 && gamepages == 1)
    {
        iShowBMP2(551, 391, "img\\start_button.bmp",255);
    }
    else if (hover == 2 && gamepages == 1){
        iShowBMP2(546, 310, "img\\score_button.bmp",255);
    }
    else if (hover == 3 && gamepages == 1){
        iShowBMP2(560, 226, "img\\quit_button.bmp",255);
    }
	

}


#endif