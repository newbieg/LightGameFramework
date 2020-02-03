#include "sprite.h"
#include <iostream>

SDL_Window* wind;
const int windX = 1200;
const int windY = 700;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;

using namespace std;

int main(int argc, char ** argv)
{
	string basePath = SDL_GetPrefPath("Softfoot_Software", "LGF_buttonBuilder");
	string baseFontPath = basePath + "res/Acme/Acme-Regular.ttf";
	initFramework();
	window wind("Template Window", windX, windY, SDL_WINDOW_RESIZABLE);

	SDL_Surface* screen;
	wind.linkScreen(&screen);

	item face[BTN_DEAD + 1] = {item(0xFF559955, 200, 200, 80, 50), item(0xFF99BB99, 300, 200, 80, 50), item(0xFF44aa44, 200, 300, 80, 50), item(0xFF339933, 300, 300, 80, 50), item(0xFF443344, 200, 400, 80, 50), item(0xFF333333, 300, 400, 80, 50)};
	button active;


	group items;
	items.add(&active);
	for(int i = 0; i <= BTN_DEAD; i ++)
	{
		items.add(&face[i]);
		active.setImage(i, face[i].getImage());
	}

	bool run = true;
	speed spd;
	spd.fps = 30;
	spd.redux = SDL_GetTicks();
	while(run)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev) != 0)
		{
			run = wind.handleEvent(ev);
			active.eventCheck(ev);
			if(ev.type == SDL_KEYDOWN)
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

//		spd.printFPS(screen, "res/Acme/Acme-Regular.ttf",  20, 20);
		items.draw(screen);
		wind.draw();
		spd.limitFPS();
		spd.fc++;
	}

	wind.close();

	closeFramework();
	return 0;
}



