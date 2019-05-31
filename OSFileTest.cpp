/* Testing multi-OS programming between APPLE, WINDOWS, ANDROID< and LINUX*/

#include <iostream>
#include "sprite.h"

using namespace std;

int main()
{
#ifdef __unix__
	#define pathHash '/'
	cout << "Linux or Unix-type OS detected.\n";
	cout << "SDLPATH:" << SDL_GetPrefPath("yourGroup", "appName") << endl;

#elif __ANDROID__
	#define pathHash '/'
	cout << "Linux-Non-Unix type system detected.\n";
	cout << "SDLPATH:" << SDL_GetPrefPath("yourGroup", "appName") << endl;

#elif defined(__LINUX__)
	#define pathHash '/'
	cout << "Linux-Non-Unix type system detected.\n";
	cout << "SDLPATH:" << SDL_GetPrefPath("yourGroup", "appName") << endl;

#elif defined(_WIN32) || defined(WIN32)
	#define pathHash '\\'
	cout << "Windows-type system detected.\n";
	cout << "SDLPATH:" << SDL_GetPrefPath("yourGroup", "appName") << endl;
#elif defined(__APPLE__)
	#define pathHash '/'
	cout << "APPLE type system detected.\n";
	cout << "SDLPATH:" << SDL_GetPrefPath("yourGroup", "appName") << endl;
	
#endif


}
