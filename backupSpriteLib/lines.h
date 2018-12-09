#ifndef SOMECODEFORLINESANDPIXELSINSDL2
#define SOMECODEFORLINESANDPIXELSINSDL2

#include <SDL2/SDL.h>


// test that a point is inside a surface drawing area
bool isInside(int x, int y, SDL_Surface* surf);

void swap(int & x, int & y);

void donoth();

// draw a pixel of color (which should already be in the correct format) onto dest at (x, y)
void putPixel(SDL_Surface* dest, int x, int y, unsigned int color);

// draw a line on dest from (x1, y1) to x2, y2)
void drawLine(SDL_Surface *dest, unsigned int color, int x1, int y1, int x2, int y2);

void drawBox(SDL_Surface * dest, SDL_Rect rect, unsigned int color);
void drawBox(SDL_Surface * dest, SDL_Rect rect, int r, int g, int b);


#endif
