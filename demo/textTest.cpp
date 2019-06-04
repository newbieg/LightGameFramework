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
	wind = SDL_CreateWindow("Write Something",SDL_WU, SDL_WU , windX, windY, SDL_WINDOW_OPENGL);
	if(wind == NULL)
	{
		cout << "Could not create Window\n";
	}

	SDL_Surface* screen = SDL_GetWindowSurface(wind);
	SDL_UpdateWindowSurface(wind);

	textBox bob;
	bob.setBoundary(10, 10, windX - 20, windY - 20);
	bob.setBkgColor(20, 20, 20);

	bool run = true;
	speed spd;
	spd.fps = 15;
	spd.redux = SDL_GetTicks();

	while(run)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev) != 0)
		{
			if(ev.type == SDL_QUIT)
			{
				run = false;
				break;
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
				}
			}
			else if(ev.type == SDL_MOUSEBUTTONDOWN)
			{
			}
			bob.handleEvent(ev);
		}
		bob.draw(screen);
		spd.printFPS(screen, "res/Acme/Acme-Regular.ttf",  windX - 140, windY - 30);
		SDL_UpdateWindowSurface(wind);
		spd.limitFPS();
		spd.fc++;
	}

	SDL_FreeSurface(screen);

	SDL_DestroyWindow(wind);

	SDL_Quit();
	return 0;
}



