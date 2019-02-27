#include "sprite.h"

#include <iostream>

SDL_Window* wind;
const int windX = 1200;
const int windY = 700;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;


using namespace std;




int main(int argc, char ** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	wind = SDL_CreateWindow("Title",SDL_WU, SDL_WU , windX, windY, SDL_WINDOW_OPENGL);
	if(wind == NULL)
	{
		cout << "Could not create Window\n";
	}

	SDL_Surface* screen = SDL_GetWindowSurface(wind);
	SDL_UpdateWindowSurface(wind);

	bool run = true;
	speed spd;
	spd.fps = 30;
	spd.redux = SDL_GetTicks();
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
					case SDLK_SPACE:
					break;
				}

			}
			else if(ev.type == SDL_MOUSEBUTTONDOWN)
			{

			}

		}
		// Draw to screen here.
		SDL_FillRect(screen, NULL, 0xffffff);

		spd.printFPS(screen, "res/Acme/Acme-Regular.ttf",  20, 20);
		SDL_UpdateWindowSurface(wind);
		spd.limitFPS();
		spd.fc++;
	}

	SDL_FreeSurface(screen);

	SDL_DestroyWindow(wind);

	SDL_Quit();
	return 0;
}



