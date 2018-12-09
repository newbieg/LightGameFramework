#include "lines.h"
#include <math.h>


void swap(int & x, int & y)
{
	int hold = x;
	x = y;
	y = hold;

}

bool isInside(int x, int y, SDL_Surface * surf)
{
	if((x >= 0 && x < surf->w) && (y >= 0 && y < surf->h))
	{
		return true;
	}
	return false;
}

void putPixel(SDL_Surface* dest, int x, int y, unsigned int color)
{
	if(isInside(x, y, dest))
	{
		((unsigned int *)dest->pixels)[(y*dest->pitch/4)+x] = color;
	}
}


void drawLineLow(SDL_Surface * dest, unsigned int color, int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if(dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	int D = 2*dy - dx;
	int y = y0;
	for(int x = x0; x < x1; x ++)
	{
		putPixel(dest, x, y, color);
		if(D > 0)
		{
			y = y + yi;
			D = D - 2*dx;
		}
		D = D + 2*dy;
	}
}
void drawLineHigh(SDL_Surface * dest, unsigned int color, int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if(dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = 2*dx - dy;
	int x = x0;

	for(int y = y0; y < y1; y ++)
	{
		putPixel(dest, x, y, color);
		if(D > 0)
		{
			x = x + xi;
			D = D - 2*dy;
		}
		D = D + 2*dx;
	}
	

}

void drawLine(SDL_Surface * dest, unsigned int color, int x0, int y0, int x1, int y1)
{
	if(abs(y1-y0) < abs(x1-x0))
	{
		if(x0 > x1)
		{
			drawLineLow(dest,color, x1, y1, x0, y0);
		}
		else
		{
			drawLineLow(dest, color, x0, y0, x1, y1);
		}
	}
	else
	{
		if(y0 > y1)
		{
			drawLineHigh(dest,color, x1, y1, x0, y0);
		}
		else
		{
			drawLineHigh(dest, color, x0, y0, x1, y1);
		}
	}

}


void drawBox(SDL_Surface * dest, SDL_Rect rect, unsigned int color)
{
	drawLine(dest, color, rect.x, rect.y, rect.x, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y, rect.x + rect.w, rect.y);
	drawLine(dest, color, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
}

void drawBox(SDL_Surface * dest, SDL_Rect rect, int r, int g, int b)
{
	unsigned int color = SDL_MapRGB(dest->format, r, g, b);
	drawLine(dest, color, rect.x, rect.y, rect.x, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y, rect.x + rect.w, rect.y);
	drawLine(dest, color, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
}




