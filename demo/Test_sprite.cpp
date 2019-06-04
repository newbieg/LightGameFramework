#include <SDL2/SDL.h>
#include "../sprite.h"

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
	wind = SDL_CreateWindow("Title",SDL_WU, SDL_WU , windX, windY, SDL_WINDOW_OPENGL);
	if(wind == NULL)
	{
		cout << "Could not create Window\n";
	}
	TTF_Init();

	SDL_Surface* screen = SDL_GetWindowSurface(wind);
	SDL_UpdateWindowSurface(wind);
	unsigned int RED = SDL_MapRGB(screen->format,255, 10, 10);

	static int userX = windX/2 - 50;
	static int userY = windY/2 - 100;

	group resMan; // resource manager (free all at end.)
	
	item man(windX/2, windY/2, 100, 100);
	man.setImage("res/dot.png");
	unsigned int BLUE = SDL_MapRGB(man.getImage()->format, 10, 10, 255);
	drawCircle(man.getImage(), BLUE, 15.0, 15.0, 6);
	resMan.add(&man);


	item grass(0,0,TILESIZE, TILESIZE);
	resMan.add(&grass);
	grass.setImage("res/grass.png");

	board lvl;
	resMan.add(&lvl);
	lvl.walk.add(&man);
	lvl.setDimensions(50, 50);
//	lvl.setPadding(-1, -4, 90, 90);
	lvl.setCamera(0, 0, windX, windY);
	lvl.fillBoard(grass);
	lvl.driven.add(&man);

//	item tank(BLUE, rand() % lvl.getPos().w, rand() % lvl.getPos().h, 30, 20);
	item tank(BLUE, 100, 100, 30, 20);
	lvl.solids.add(&tank);
	lvl.driven.add(&tank);


	bool run = true;
	bool left = false, right = false, up= false, down = false;
	int walk = 3;
	speed limit;
	limit.fps = 30;
	limit.redux = SDL_GetTicks();
	double phi = 0;
	while(run)
	{

		SDL_Event ev;

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
					case SDLK_SPACE:
						walk = 3;
					break;
					case SDLK_DOWN:
						down = true;
					break;
					case SDLK_UP:
						up = true;
					break;
					case SDLK_LEFT:
						left = true;
					break;
					case SDLK_RIGHT:
						right = true;
					break;
				}
			}
			else if(ev.type == SDL_KEYUP)
			{
				switch(ev.key.keysym.sym)
				{
					case SDLK_SPACE:
						walk = 8;
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
		int oldx = lvl.getCamera()->x;
		int oldy = lvl.getCamera()->y;

		if(up)
		{
			int changeX = xFromPolar(walk, phi);
			int changeY = yFromPolar(walk, phi);
			lvl.move(changeX, changeY);
//			lvl.driven.move( -1 * changeX, -1 * changeY);
//			man.move(changeX, changeY);
		}
		if(down)
		{
			int changeX = xFromPolar(walk, phi);
			int changeY = yFromPolar(walk, phi);
			lvl.move(-1 * changeX, -1 * changeY);
//			lvl.driven.move(1 * changeX, 1 * changeY);
//			man.move( -1 * changeX, -1 * changeY);
		}
		if(left)
		{
			phi += .05;
		}
		if(right)
		{
			phi -= .05;
		}

		lvl.draw(screen);
//		man.draw(screen);
		lvl.driven.draw(screen);
		SDL_Rect temp = man.getPos();
		temp.x = temp.x + temp.w/2;
		temp.y = temp.y + temp.h/2;

		
		drawLine(screen,RED, temp.x, temp.y, temp.x + xFromPolar(walk, phi), temp.y + yFromPolar(walk, phi));



		limit.fc ++;
		limit.printFPS(screen, "res/Crete_Round/CreteRound-Regular.ttf", 10, 10);
		limit.printGT(screen, "res/Crete_Round/CreteRound-Regular.ttf", 200, 10);
		SDL_UpdateWindowSurface(wind);
		limit.limitFPS();
	}

	resMan.free();
	SDL_DestroyWindow(wind);

	SDL_Quit();
	return 0;
}



