/* A program that takes a text type file and displays one word at a time to the user. 
 * This word is the only thing on the screen to reduce distractions.
 * the user is given the option of speeding up how quickly the words are displayed as they are
 * reading, or choosing a speed before starting. 
 * good luck and may the force be with you.
 * */

#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
#include "sprite.h"

using namespace std;

int atoi(char * wordToNumber)
{
	int number = 0;
	for(int i = 0; i < strlen(wordToNumber); i ++)
	{
		number *= 10;
		int temp = wordToNumber[i] - '0';
		if(temp >= 0 && temp <= 9)
		{
			number += temp;

		}
	}
	return number;
}

vector <string> getBook(string fileName)
{
	ifstream inf(fileName.c_str());
	vector <string> book;
	while(inf.good())
	{
		string line;
		getline(inf, line);
		book.push_back(line);
	}
	return book;
}

int main(int argc, char ** argv)
{

	cout << argv[0] << " a simple book reader\n";
	string fileName;
	int skipper = 10;
	speed spd;
	spd.fps = 60;
	for(int i = 1; i < argc; i ++)
	{
		if(strcmp(argv[i], "-speed") == 0 || strcmp(argv[i], "-s") == 0)	
		{
			i ++;
			spd.fps = atoi(argv[i]);
		}
		else if(strcmp(argv[i], "-file") == 0 || strcmp(argv[i], "-f") == 0)
		{
			i ++;
			fileName = argv[i];
		}
	}

	if(fileName.empty())
	{
		cout << "File Name not specified, use -f fileName\n";
		exit(0);
	}

	// void * hndl = getBook; set this up so we have a final handle if file could not be read
	// by any other of the plugins available.
	// while(getBookHNDL() == emptyVector) increment hndls...
	vector <string> book = getBook(fileName);

	if(book.empty())
	{
		cout << "Couldn't read the provided file, perhaps no read permisions, \nor the file type is not supported.\n";
		exit(0);
	}
	
	initFramework();
	window wind("ASBR - A Simple Book Reader", 800, 600, SDL_WINDOW_RESIZABLE);
	wind.show();
	cout << wind.getOpacity();
	SDL_Delay(3000);
	SDL_Surface * screen;
	screen = wind.getImage();
	stringstream ss;
	//txt display("hello world", "res/Noto_Serif/NotoSerif-Regular.ttf", 100, 100);
	txt display("hello world", "res/Roboto/Roboto-Regular.ttf", 100, 100);
	display.setFontSize(80);
	display.fullRender();

	int lineCount = 1;
	bool run = true;
	int wait = 400;
	while(run)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					run = false;
					break;
				case SDL_DROPFILE:

					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_w:
							if(SDL_GetModState() & KMOD_ALT)
							{
								run = false;
								wind.close();
								closeFramework();
								exit(0);
							}
							break;
						case SDLK_f:
							wind.toggleFS();
							screen = wind.getImage();
							break;
						case SDLK_UP:
							wind.addOpacity(0.05);
							break;
						case SDLK_DOWN:
							wind.addOpacity(-0.05);
							break;
						case SDLK_LEFT:
							skipper ++;
							if(skipper > spd.fps)
							{
								skipper = spd.fps;
							}
								
							break;
						case SDLK_RIGHT:
							skipper --;
							if(skipper < 1)
							{
								skipper = 1;
							}
							break;
					}

					break;

			}
		}

		string word;
		if(!(spd.fc % skipper))
		{
			ss >> word;
			if(word.empty() && lineCount < book.size() - 1)
			{
				ss.str("");
				ss.clear();
				ss << book[lineCount];
				ss >> word;
				lineCount ++;
				if(lineCount >= book.size() - 1)
				{
					run = false;
				}
			}


			SDL_FillRect(screen, NULL, 0x0ff);
			if(!(word.empty()))
			{
				cout << word << " ";
				display.setText(word);
				display.fullRender();
	
				if(display.getImage() != NULL)
				{
					display.draw(screen);
				}
			}
		}

		wind.draw();
		spd.limitFPS();
		spd.fc++;

	}



	wind.close();
	closeFramework();

}
