/* risk.cpp
 * opens a risk board and places soldier items on each country.
 * Not a fully functional game, but demonstrates how
 * any item can be checked for a click,
 * how to use the  and was used in 
 * testing a button class which is now functional.
 * Was written before items could load their own 
 * images, so the code is verbose for no reason.
 */

#include "sprite.h"
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>


const int windX = 1227;
const int windY = 628;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED; // too lazy to type this twice in a row. I mean, seriously, who has the patience to do this every time they open a window in a template...
const int soldierW = 25;
const int soldierH = 15;

void credits();


class soldier: public item
{
	public:
		soldier();
		~soldier();
		virtual SDL_Rect update();
		explicit soldier(unsigned int color, int x, int y, int w, int h) : item(color, x, y, w, h) { 
			team = 0; 
			hp = 0;
	       	};
		explicit soldier(int x, int y, int w, int h) : item(x, y, w, h) 		{
			team = 0; 
			hp = 0;
		};
		void add(int count);
		void sub(int count);
		void clear();
		int getTeam();
		void setTeam(int player);
		int getHP() {
			return hp;
		}
		bool sellect(bool state)
		{
			sellected = state;
		}

	private:
		int hp; // number of soldiers in soldier position.
		int team;
		bool sellected;
		group canAttack; // Not set yet

};

soldier::soldier()
{
	this->clear();
	hp = 0;
	team = 0;
	sellected = false;
}
soldier::~soldier()
{

}

SDL_Rect soldier::update()
{
	std::stringstream ss;
	ss << hp;
	std::string temp;
	ss >> temp;
	unsigned int white = SDL_MapRGB(image->format, 255, 255, 255);
	unsigned int red = SDL_MapRGB(image->format, 255, 25, 25);
	unsigned int blue = SDL_MapRGB(image->format, 25, 255, 25);
	unsigned int green = SDL_MapRGB(image->format, 25, 25, 225);
	unsigned int yellow = SDL_MapRGB(image->format, 225, 225, 25);
	unsigned int color = white;
	if(team == 0)
	{
		color = white;
	}
	if(team == 1)
	{
		color = red;
	}
	else if(team == 2)
	{
		color = blue;
	}
	else if(team == 3)
	{
		color = green;
	}
	else if(team == 4)
	{
		color = yellow;
	}
	SDL_FillRect(this->image, NULL, color);

	txt num(temp, "res/Acme/Acme-Regular.ttf", 2, 0);
	num.draw(this->image);

	return this->rect;
	
}

void soldier::add(int count)
{
	hp += count;
	if(hp <= 0)
	{
		hp = 0;
	}
}

void soldier::sub(int count)
{
	hp -= count;
	if(count < 0)
	{
		hp = 0;
	}
}

void soldier::clear()
{
	hp = 0;
	team = 0; // neutral
}

int soldier::getTeam()
{
	return team;
}

void soldier::setTeam(int player)
{
	team = player;
}




unsigned int distWhite(SDL_Surface* screen, int r, int g, int b, int someQuotient)
{
	return SDL_MapRGB(screen->format, r+(255-r)/someQuotient, g+(255-g)/someQuotient, b+(255-b)/someQuotient);

}

using namespace std;

string fontName = "res/Acme/Acme-Regular.ttf";	
soldier soldierBank[5];
soldier doNothing;
soldier * handleClick(soldier * active, soldier* lastClicked, int team, int gameState)
{
	if(lastClicked == &doNothing)
	{
		return active;
	}
	team = team%5;
	if((gameState % 2 ) && soldierBank[team].getHP() > 0)
	{
		if(active->getTeam() == team || active->getTeam() == 0 || active->getHP() <= 0)
		{
			active->setTeam(team);
			active->add(1);
			soldierBank[team].sub(1);
		}
	}
	else if(gameState%2 == 0)
	{
		// soldiers being moved to enemy country on attack
		if(lastClicked->getTeam() == team && active->getTeam() != team && lastClicked->getHP() > 1 && active->getHP() > 0)
		{
			active->sub(1);
			lastClicked->sub(1);
			return lastClicked;
		}
		// soldiers capturing an enemy country
		if(lastClicked->getTeam() == team && active->getTeam() != team && active->getHP() <= 0 && lastClicked->getHP() >= 1)
		{
			active->setTeam(team);
			lastClicked->sub(1);
			return lastClicked;
		}
		// moving between friendly countries
		else if(lastClicked->getTeam() == team && active->getTeam() == team && lastClicked->getHP() > 1)
		{
			if(lastClicked->getPos().x == active->getPos().x && lastClicked->getPos().y == active->getPos().y)
			{
				return &doNothing;
			}
			lastClicked->sub(1);
			active->add(1);
			return lastClicked;
		}
	}
	return active;
}

int main()
{
	int team = 0;
	soldierBank[0].add(15);
	soldierBank[1].add(15);
	soldierBank[2].add(15);
	soldierBank[3].add(15);
	soldierBank[4].add(15);

	SDL_Init(SDL_INIT_EVERYTHING);
	if(! (IMG_Init(IMG_INIT_PNG)))
	{
		cout << "Could not init png loading\n";
		exit(0);
	}
	if(TTF_Init() == -1)
	{
		cout << "Could not init FONTSs\n";
		exit(0);
	}
	// set up window and images
	SDL_Window* wind = NULL;
	wind = SDL_CreateWindow("Title", SDL_WU, SDL_WU, windX, windY, SDL_WINDOW_OPENGL);
	SDL_Surface* screen = SDL_GetWindowSurface(wind);

	//set up some colors
	unsigned int white = SDL_MapRGB(screen->format, 255, 255, 255);
	unsigned int red = SDL_MapRGB(screen->format, 255, 25, 25);
	unsigned int blue = SDL_MapRGB(screen->format, 25, 255, 25);
	unsigned int green = SDL_MapRGB(screen->format, 25, 25, 225);

	// BUTTON IMAGES
	SDL_Surface* btnu = IMG_Load("res/btnu.png");
	SDL_Surface* btnd = IMG_Load("res/btnd.png");
	SDL_Surface* btnh = IMG_Load("res/btnh.png");
	SDL_Surface* btndd = IMG_Load("res/btnd.png");

	button gimp;
	gimp.setSize(100, 100);
	gimp.setImage(BTN_UP, btnu);
	gimp.setImage(BTN_HOVER, btnh);
	gimp.setImage(BTN_DOWN, btnd);
	gimp.setImage(BTN_CLICK, btnu);
	gimp.setImage(BTN_DBCLICK, btnu);
	gimp.setPos(200, 200);

	// Cursors
	SDL_Cursor *cursor = NULL;
	SDL_Cursor *rcursor = NULL;
	SDL_Cursor *gcursor = NULL;
	SDL_Cursor *bcursor = NULL;
	SDL_Cursor *ycursor = NULL;
	SDL_Surface* cursorSurf = NULL;
	cursorSurf = IMG_Load("res/cursor.png");
	cursor = SDL_CreateColorCursor(cursorSurf, 4, 4);
	cursorSurf = IMG_Load("res/cursorRed.png");
	rcursor = SDL_CreateColorCursor(cursorSurf, 4, 4);
	cursorSurf = IMG_Load("res/cursorGreen.png");
	gcursor = SDL_CreateColorCursor(cursorSurf, 4, 4);
	cursorSurf = IMG_Load("res/cursorBlue.png");
	bcursor = SDL_CreateColorCursor(cursorSurf, 4, 4);
	cursorSurf = IMG_Load("res/cursorYellow.png");
	ycursor = SDL_CreateColorCursor(cursorSurf, 4, 4);
	SDL_SetCursor(cursor);
	vector <SDL_Cursor*> cursors = {cursor, rcursor, gcursor, bcursor, ycursor};
	// RESOURCES: Load the images and colors of items/boards.
	group bkg;
	item backGroundColor(white, 0,0,windX, windY);
	item backGroundIMG(0,0, windX, windY);
	backGroundIMG.setImage("res/map_0.png");
	bkg.add(&backGroundColor);
	bkg.add(&backGroundIMG);

	group board_1;
	soldier box1(70, 66, soldierW, soldierH);
	soldier box2(169, 61, soldierW, soldierH);
	soldier box3(132, 108, soldierW, soldierH);
	soldier box4(114, 165, soldierW, soldierH);
	soldier box5(223, 115, soldierW, soldierH);
	soldier box6(303, 115, soldierW, soldierH);
	soldier box7(199, 180, soldierW, soldierH);
	soldier box8(132, 240, soldierW, soldierH);
	soldier box9(421, 30, soldierW, soldierH);
	soldier box10(421, 30, soldierW, soldierH);
	soldier box11(265, 325, soldierW, soldierH);
	soldier box12(274, 426, soldierW, soldierH);
	soldier box13(333, 403, soldierW, soldierH);
	soldier box14(289, 499, soldierW, soldierH);
	soldier box15(490, 78, soldierW, soldierH);
	soldier box16(513, 115, soldierW, soldierH);
	soldier box17(555, 157, soldierW, soldierH);
	soldier box18(603, 78, soldierW, soldierH);
	soldier box19(598, 127, soldierW, soldierH);
	soldier box20(631, 157, soldierW, soldierH);
	soldier box21(678, 111, soldierW, soldierH);
	soldier box22(781, 85, soldierW, soldierH);
	soldier box23(793, 151, soldierW, soldierH);
	soldier box24(714, 202, soldierW, soldierH);
	soldier box25(855, 67, soldierW, soldierH);
	soldier box26(949, 66, soldierW, soldierH);
	soldier box27(948, 112, soldierW, soldierH);
	soldier box28(955, 151, soldierW, soldierH);
	soldier box29(945, 201, soldierW, soldierH);
	soldier box30(862, 247, soldierW, soldierH);
	soldier box31(1063, 70, soldierW, soldierH);
	soldier box32(1107, 189, soldierW, soldierH);
	soldier box33(969, 282, soldierW, soldierH);
	soldier box34(544, 273, soldierW, soldierH);
	soldier box35(633, 226, soldierW, soldierH);
	soldier box36(708, 318, soldierW, soldierH);
	soldier box37(634, 352, soldierW, soldierH);
	soldier box38(639, 445, soldierW, soldierH);
	soldier box39(739, 441, soldierW, soldierH);
	soldier box40(1018, 355, soldierW, soldierH);
	soldier box41(1138, 381, soldierW, soldierH);
	soldier box42(1039, 477, soldierW, soldierH);
	soldier box43(1131, 471, soldierW, soldierH);
	board_1.add(&box1);
	board_1.add(&box2);
	board_1.add(&box3);
	board_1.add(&box4);
	board_1.add(&box5);
	board_1.add(&box6);
	board_1.add(&box7);
	board_1.add(&box8);
	board_1.add(&box9);
	board_1.add(&box10);
	board_1.add(&box11);
	board_1.add(&box12);
	board_1.add(&box13);
	board_1.add(&box14);
	board_1.add(&box15);
	board_1.add(&box16);
	board_1.add(&box17);
	board_1.add(&box18);
	board_1.add(&box19);
	board_1.add(&box20);
	board_1.add(&box21);
	board_1.add(&box22);
	board_1.add(&box23);
	board_1.add(&box24);
	board_1.add(&box25);
	board_1.add(&box26);
	board_1.add(&box27);
	board_1.add(&box28);
	board_1.add(&box29);
	board_1.add(&box30);
	board_1.add(&box31);
	board_1.add(&box32);
	board_1.add(&box33);
	board_1.add(&box34);
	board_1.add(&box35);
	board_1.add(&box36);
	board_1.add(&box37);
	board_1.add(&box38);
	board_1.add(&box39);
	board_1.add(&box40);
	board_1.add(&box41);
	board_1.add(&box42);
	board_1.add(&box43);
	board_1.save("soldierMap.txt");
	txt intro1("A Game Of", fontName, 110, 10);
	txt intro2("RISK!",fontName, 350, 310);
	intro1.setFontSize(240);
	intro2.setFontSize(280);
	intro1.fullRender();
	intro2.fullRender();
	board_1.add(&intro1);
	board_1.add(&intro2);

	SDL_Event ev;
	bool run = true;

	// DRAW THE INTRO
	SDL_SetCursor(cursors[1]);
	bkg.draw(screen);
	board_1.update();
	board_1.draw(screen);

	group testGrp;
	soldier test(20, 20, 30, 30);
	testGrp.add(&test);
	testGrp.update();
	testGrp.draw(screen);
	gimp.draw(screen);

	SDL_UpdateWindowSurface(wind);
	SDL_Delay(2000);
	board_1.remove(intro1);
	board_1.remove(intro2);

	int ticks = 0;
	int gameState = 0;
	soldier* lastSoldier = (soldier *) board_1.getItem(0);
	soldier* inc = (soldier *) board_1.getItem(0);
	while(run)
	{
		while(SDL_PollEvent(&ev) != 0)
		{
			if(ev.type == SDL_QUIT)
			{
				run = false;
			}
			else if(ev.type == SDL_KEYDOWN)
			{
				switch(ev.key.keysym.sym)
				{
					case SDLK_SPACE:
					gameState ++;
					if(gameState %2)
					{
						team ++;
					}
					break;
				}

			}
			else if(ev.type == SDL_MOUSEBUTTONDOWN)
			{
				group clicked = board_1.getClicked(ev.button.x, ev.button.y);
				if(!(clicked.isEmpty()))
				{
					for(int i = 0; i < clicked.size(); i ++)
					{
						inc = (soldier*) clicked.getItem(i);
						lastSoldier = handleClick(inc, lastSoldier, team, gameState);
					}
				}

			}
			gimp.eventCheck(&ev);
		}
		SDL_SetCursor(cursors[team % 5]);
		bkg.draw(screen);
		board_1.update();
		board_1.draw(screen);
		gimp.draw(screen);

		SDL_UpdateWindowSurface(wind);
		speedLimit(10, ticks);
	}

	SDL_FreeSurface(screen);
	SDL_DestroyWindow(wind);
	SDL_Quit();
	credits();
	return 0;
}

void credits()
{
	cout << "This game uses some resources from external sources.\
		\nCredits:\n Wikipedia:\n\t for their wonderfull and open licensed image of a risk board found here:\n\t\t https://en.wikipedia.org/wiki/Risk_(game)\n";
	cout << "\nhttps://piq.codeus.net/picture/42831/Mouse-pointer for the great hand pointer/cursor, I barely modified it to show teams\n\n";
}

