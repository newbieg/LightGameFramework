#include "sprite.h"

#include <iostream>

SDL_Window* wind;
const int windX = 1200;
const int windY = 700;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;


using namespace std;




int main(int argc, char ** argv)
{
	initFramework();
	window wind("Template Window", windX, windY, SDL_WINDOW_RESIZABLE);

	SDL_Surface* screen;
	wind.linkScreen(&screen);

	item tom(200, 100, 50, 40);
	
	bool run = true;
	speed spd;
	spd.fps = 30;
	spd.redux = SDL_GetTicks();
	while(run)
	{
		SDL_Event ev;
		if(SDL_PollEvent(&ev) != 0)
		{
			wind.handleEvent(ev);
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
						wind.toggleFS();

					case SDLK_SPACE:

					break;
					case SDLK_RIGHT:
					{
						tom.move(1,0);
					}
				}

			}
			else if(ev.type == SDL_KEYUP)
			{
				if(ev.key.keysym.sym == SDLK_f)
				{
				}

			}
			else if(ev.type == SDL_MOUSEBUTTONDOWN)
			{

			}

		}


		// Draw to screen here.
		SDL_FillRect(screen, NULL, 0xffffff);

		tom.draw(screen);
		spd.printFPS(screen, "res/Acme/Acme-Regular.ttf",  20, 20);
		wind.draw();
	//	spd.limitFPS();
		spd.fc++;
	}

	SDL_FreeSurface(screen);

	wind.close();

	SDL_Quit();
	return 0;
}



