#include "../../sprite.h"
#include <iostream>

SDL_Window* wind;
const int windX = 1200;
const int windY = 800;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;

enum toolshed
{
	PENCIL,
	BRUSH,
	BUCKETFILL,
	RECTANGLEFILL,
	SELLECT,
	ERASER
};

using namespace std;

int main(int argc, char ** argv)
{

	int activeTool = RECTANGLEFILL;
	Uint32 activeColor = 0xFFFF4444; // Dark Red

	string basePath = SDL_GetPrefPath("Softfoot_Software", "LGF_buttonBuilder");
	string baseFontPath = basePath + "res/Acme/Acme-Regular.ttf";
	initFramework();
	window wind("Button Builder V_0.0.001", windX, windY, SDL_WINDOW_RESIZABLE);
	window tools("Tools", windX/4, windY, 0);

	SDL_Surface* mainScreen;
	wind.linkScreen(&mainScreen);
	SDL_Surface* toolScreen;
	tools.linkScreen(&toolScreen);

	button active;
	active.setPos(30, 30);
	item face[BTN_DEAD + 1] = {item(0xFF559955, 50, 250, 320, 200), item(0xFF99BB99, 400, 250, 320, 200), item(0xFF44aa44, 750, 250, 320, 200), item(0xFF339933, 50, 480, 320, 200), item(0xFF443344, 400, 480, 320, 200), item(0xFF333333, 750, 480, 320, 200)};
	txt hover("Hover", baseFontPath, 0, 0); 
	hover.stamp(face[BTN_HOVER].getImage(), 30, 30);
	txt deflt("Default", baseFontPath, 0, 0);
	deflt.stamp(face[BTN_UP].getImage(), 30, 30);
	txt down("Down", baseFontPath, 0,0);
	down.stamp(face[BTN_DOWN].getImage(), 30, 30);
	txt clicked("Clicked", baseFontPath, 0,0);
	clicked.stamp(face[BTN_CLICK].getImage(), 30, 30);

	group items;
	items.add(&active);
	for(int i = 0; i <= BTN_DEAD; i ++)
	{
		items.add(&face[i]);
		active.setImage(i, face[i].getImage());
	}
	
	// tools
	item drawRect;
	drawRect.hide();
	drawRect.setColor(activeColor);

	bool run = true;
	speed spd;
	spd.fps = 30;
	spd.redux = SDL_GetTicks();
	bool buttonDown = false;
	SDL_Rect points;
	while(run)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev) != 0)
		{
			run = wind.handleEvent(ev);
			tools.handleEvent(ev);
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
				drawRect.free();
				drawRect.setPos(ev.button.x, ev.button.y);
				drawRect.setSize(0,0);
				drawRect.hide();
				buttonDown = true;
				points.x = ev.button.x;
				points.y = ev.button.y;
				points.w = 0;
				points.h = 0;
			}
			else if(ev.type == SDL_MOUSEBUTTONUP)
			{
				// stamp the surfaces that is applies to, then hide the rect
				buttonDown = false;
				drawRect.hide();

			}
			else if(ev.type == SDL_MOUSEMOTION)
			{
				if(buttonDown)
				{
					//drawRect.free();
					points.w = ev.motion.x;
					points.h = ev.motion.y;
					if(points.x > points.w)
					{
						drawRect.setPos(points.w, drawRect.getPos().y);
						drawRect.setSize(points.x - points.w, drawRect.getPos().w);
					}
					else
					{
						points.w -= points.x;
						drawRect.setSize(points.w, drawRect.getPos().h);
					}
					if(points.y > points.h)
					{
						drawRect.setPos(drawRect.getPos().x, points.h);
						drawRect.setSize(drawRect.getPos().w, points.y - points.h);

					}
					else
					{
						points.h -= points.y;
						drawRect.setSize(drawRect.getPos().w, points.h);

					}
					
					drawRect.show();
				}

			}
		}
		// Draw to mainScreen here.
		SDL_FillRect(mainScreen, NULL, 0xffffff);
		SDL_FillRect(toolScreen, NULL, 0xffffff);

//		spd.printFPS(mainScreen, "res/Acme/Acme-Regular.ttf",  20, 20);
		items.draw(mainScreen);
		drawRect.draw(mainScreen);
		wind.draw();
		tools.draw();
		spd.limitFPS();
		spd.fc++;
	}

	wind.close();
	tools.close();

	closeFramework();
	return 0;
}



