#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main()
{
	cout << SDL_GetPrefPath("Softfoot_Software", "LGF_buttonBuilder");
}
