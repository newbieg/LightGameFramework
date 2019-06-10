/* Testing multi-OS programming between APPLE, WINDOWS, ANDROID< and LINUX
 * These may be usefull if at some point I have to create my own cross-
 * platform source code. First thing that springs to mind is a directory
 * lister option so that I could implement a plugin-folder...
 * */

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
#elif defined(__APPLE__ || __MACOSX__ || __MAC__) 
	#define pathHash '/'
	cout << "APPLE type system detected.\n";
	cout << "SDLPATH:" << SDL_GetPrefPath("yourGroup", "appName") << endl;
	
#endif

#ifdef __GNUC__
	cout << "Compiled using gnu gcc compiler\n";
#elif defined (__clang__)
	cout << "Compiled using the CLang compiler\n";
#elif defined(_MSC_VER)
	cout << "Compiled using Visual Studio C++\n"

#endif
	/* other __useful__ definitions... posibly...
	 * __SOLARIS__ : LP64
	 * __WATCOM__ I had no idea that WATCOM was still a thing, just a name I associated with the late 80's
	 *
	 */

}
