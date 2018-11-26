#include <SDL2/SDL.h>
#include "sprite.h"
#include "lines.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>

SDL_Window* wind;
const int windX = 880;
const int windY = 620;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;


using namespace std;




int main(int argc, char ** argv)
{
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	wind = SDL_CreateWindow("Title",SDL_WU, SDL_WU , windX, windY, 0);
	if(wind == NULL)
	{
		cout << "Could not create Window\n";
	}
	TTF_Init();

	SDL_Surface* screen = SDL_GetWindowSurface(wind);
	SDL_UpdateWindowSurface(wind);

	group resMan;

	item resMap;
	resMap.setImage("res/tilesetStart5_0.png");
	resMan.add(&resMap);

	tile grass2(1, 32, TILESIZE, TILESIZE);
	SDL_Rect tilegrass = {720, 128, TILESIZE, TILESIZE};
	resMap.stamp(grass2.getImage(), tilegrass, 0,0);
	resMan.add(&grass2);
	

	item grass(0,0,TILESIZE, TILESIZE);
	resMan.add(&grass);
	grass.setImage("res/grass.png");

	board lvl;
	resMan.add(&lvl);
	lvl.setDimensions(30, 30);
	lvl.setCamera(0, 0, windX, windY);
	lvl.fillBoard(grass2);

	item red;
	red.setImage("res/Tanks/Tanks/tankRed.png");
	tile tank(rand() % (windX - 100), rand() % (windY - 100), 83, 103);
	red.stamp(tank.getImage(), 0, 0);
	red.free();
	red.setImage("res/Tanks/Tanks/tracksSmall.png");
	red.stamp(tank.getImage(), 0,0);
	red.free();

	

	animation kickL;
	kickL.addImage("res/stepL1.png");
	kickL.addImage("res/stepL2.png");
	kickL.addImage("res/kickL3.png");
	resMan.add(&kickL);
	kickL.setFrameSkip(4);

	animation kickR;
	kickR.addImage("res/stepR1.png");
	kickR.addImage("res/stepR2.png");
	kickR.addImage("res/kickR3.png");
	resMan.add(&kickR);
	kickR.setFrameSkip(4);


	SDL_Rect outside = lvl.getPos();
	outside.w --;
	outside.h --;
	drawBox(lvl.getImage(), outside, 255, 255, 10);


/*
	vector <item*> resources;
	for(int i = 0; i < 300; i ++)
	{

		animation* TkickR = new animation();
		TkickR->setPos(rand()%windX, rand()%windY);
		TkickR->addImage("res/stepR1.png");
		TkickR->addImage("res/stepR2.png");
		TkickR->addImage("res/kickR3.png");
		resMan.add(TkickR);
		TkickR->setFrameSkip(4);
		resources.push_back(TkickR);

	}
*/
	animation * active = &kickL;
	bool run = true;
	bool left = false, right = false, up= false, down = false;
	int walk = 2;
	speed limit;
	limit.fps = 30;
	limit.redux = SDL_GetTicks();
	while(run)
	{

		SDL_Event ev;
		// The poll only gives about 15 events per second.
		// so that's a limiter on my camera speed... 
		// Need to monitor SDL_KEYDOWN and set bools
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
					case SDLK_w:
						if(SDL_GetModState() & KMOD_ALT)
						{
							run = false;
						}
					break;
					case SDLK_f:
						active = &kickL;
					case SDLK_SPACE:
						walk = 32;
					break;
					case SDLK_DOWN:
						down = true;
					break;
					case SDLK_UP:
						up = true;
					break;
					case SDLK_LEFT:
						left = true;
						active = &kickL;
					break;
					case SDLK_RIGHT:
						right = true;
						active = &kickR;
					break;
				}
			}
			else if(ev.type == SDL_KEYUP)
			{
				switch(ev.key.keysym.sym)
				{
					case SDLK_SPACE:
						walk = 2;
					break;
					case SDLK_DOWN:
						down = false;
					break;
					case SDLK_UP:
						up = false;
					break;
					case SDLK_LEFT:
						left = false;
					break;
					case SDLK_RIGHT:
						right = false;
					break;
				}
			}
			else if(ev.type == SDL_MOUSEBUTTONDOWN)
			{

			}
		}
		if(up)
		{
			lvl.getCamera()->y -= walk;
		}
		if(down)
		{
			lvl.getCamera()->y += walk;
		}
		if(left)
		{
			lvl.getCamera()->x -= walk;
		}
		if(right)
		{
			lvl.getCamera()->x += walk;
		}
		active->next();

		lvl.draw(screen);
		grass2.draw(screen);
		active->draw(screen);
		tank.draw(screen);
		


		limit.fc ++;
		limit.printFPS(screen, "res/Crete_Round/CreteRound-Regular.ttf", 10, 10);
		limit.printGT(screen, "res/Crete_Round/CreteRound-Regular.ttf", 200, 10);
		SDL_UpdateWindowSurface(wind);
		limit.limitFPS();
	}
	char file[] = "leef.png";
	writeImage(screen, file);


	resMan.free();
	SDL_DestroyWindow(wind);

	SDL_Quit();
	return 0;
}



