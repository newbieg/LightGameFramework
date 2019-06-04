/* A program for displaying a text file as if it were a solid
 * object, like a book, with a remembered reading position, and 
 * very little distractions
 */

#include "sprite.h"

#include <fstream>
#include <string>
#include <iostream>

SDL_Window* wind;
const int windX = 720;
const int windY = 640;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;


using namespace std;

vector <string> verse;

// Save the line number of bookName
void saveSpot(string bookName, int readingLine);
// Get the last read line of bookName (saved from previous session)
int getSpot(string bookName);
// load bookName's contents into the vector (verse) above
void loadBook(string bookName);
// Save path to current book for loading next time.
// In fact, lets save a bit more information while we're at it.
void saveStates(string bookName, int &font, int &fontSize);
// Get the name of the book we were reading last sesion
// while we're at it, lets also get the last font information.
string getStates(int &font, int &fontSize);

// Just in case Directions.txt gets deleted and the user fails
// to follow Directions.txt, Directions.txt will be rebuilt.
void buildDirections();

int main(int argc, char ** argv)
{

	initFramework();
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	window wind("Text Readah", windX, windY, SDL_WINDOW_RESIZABLE);
	
	string fonts[18] = {"res/ABeeZee/ABeeZee-Regular.ttf",\
"res/Abril_Fatface/AbrilFatface-Regular.ttf", \
"res/Acme/Acme-Regular.ttf", \
"res/Amatic_SC/AmaticSC-Bold.ttf", \
"res/Asap/Asap-Regular.ttf", \
"res/Cardo/Cardo-Regular.ttf", \
"res/Cinzel/Cinzel-Regular.ttf", \
"res/Cookie/Cookie-Regular.ttf", \
"res/Courgette/Courgette-Regular.ttf", \
"res/Crete_Round/CreteRound-Regular.ttf", \
"res/Dancing_Script/DancingScript-Regular.ttf", \
"res/Domine/Domine-Regular.ttf", \
"res/EB_Garamond/EBGaramond-Regular.ttf", \
"res/EB_Garamond/EBGaramond-SemiBold.ttf"};



	SDL_Surface* screen = wind.getImage();

	speed spd;
	spd.fps = 30;
	int fontSize = 20;
	int currentFont = 0;
	string book  = getStates(currentFont, fontSize);
	cout << book << endl;
	wind.setTitle(book);

	int cursor = getSpot(book);
	loadBook(book);
	if(verse.empty())
	{
		loadBook("Directions.txt");
		if(verse.empty())
		{
			cout << "Could not load " << book;
			cout << "And Directions.txt seems to be missing.\n";
			cout << "Please restart this program.\n";
		//	buildDirections();
		//	exit(0);
		}
	}
	SDL_Color textColor = {0x11, 0x11, 0x11, 0xff};
	txt line[31];
	for(int i = 0; i < 30; i ++)
	{
		line[i].setFont(fonts[currentFont % 14]);
		line[i].setFontSize(fontSize);
		line[i].setPos(30, i*fontSize + 60);
		if(cursor + i < verse.size())
		{
			line[i].setText(verse[cursor + i]);
			line[i].quickRender();
		}
	}
	
	bool run = true;
	spd.redux = SDL_GetTicks();
	SDL_Event ev;
	while(run)
	{
		if(SDL_WaitEvent(&ev))
		{
			wind.handleEvent(ev);
			if(ev.type == SDL_QUIT)
			{
				run = false;
			}
			else if(ev.type == SDL_DROPFILE)
			{
				saveSpot(book, cursor);
				char * fileName = ev.drop.file;
				string temp = fileName;
				book = temp;
				cursor = getSpot(book);
				loadBook(fileName);
				SDL_free(fileName);
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
					case SDLK_f:
						wind.toggleFS();
						screen = wind.getImage();
						break;
					case SDLK_SPACE:
						break;
					case SDLK_LEFT:
						if(SDL_GetModState() & KMOD_CTRL)
						{
							wind.addOpacity(.1);
						}
						else if(SDL_GetModState() & KMOD_ALT && cursor > 0)
						{
							cursor -= 100;
						}
						else if(cursor > 0)
						{
							cursor --;
						}
						if(cursor < 0)
						{
							cursor = 0;
						}
						break;
					case SDLK_RIGHT:
						if(SDL_GetModState() & KMOD_CTRL)
						{
							wind.addOpacity(-.01);
						}
						else if(SDL_GetModState() & KMOD_ALT && cursor + 100 < verse.size())
						{
							cursor += 100;
						}
						else if(cursor < verse.size())
						{
							cursor ++;
						}
					
						break;
					case SDLK_UP:
						if(SDL_GetModState() & KMOD_CTRL)
						{
							currentFont ++;
							for(int i = 0; i < 30; i ++)
							{
								currentFont %= 14;
								line[i].setFont(fonts[currentFont]);
								line[i].fullRender();
							}
						}
						else if(SDL_GetModState() & KMOD_ALT)
						{
							fontSize ++;
							for(int i = 0; i < 30; i ++)
							{
								line[i].setFontSize(fontSize);
								line[i].setPos(30, i*fontSize + 60);
								line[i].fullRender();
							}
						}
						break;
					case SDLK_DOWN:
						if(SDL_GetModState() & KMOD_CTRL)
						{
							currentFont --;
							if(currentFont < 0)
							{
								currentFont = 13;
							}	
							currentFont %= 14;
							for(int i = 0; i < 30; i ++)
							{
								line[i].setFont(fonts[currentFont]);
								line[i].fullRender();
							}
						}
						else if(SDL_GetModState() & KMOD_ALT)
						{
							fontSize --;
							for(int i = 0; i < 30; i ++)
							{
								line[i].setFontSize(fontSize);
								line[i].setPos(30, i*fontSize + 60);
								line[i].fullRender();
							}
						}
						break;
				}
	
			}
			else if(ev.type == SDL_WINDOWEVENT)
			{
				if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					screen = wind.getImage();

				}
			}

		}
		// Draw to screen here.
		SDL_FillRect(screen, NULL, 0xffffff);

		for(int i = 0; i < 30 && cursor + i < verse.size(); i ++)
		{
			line[i].setText(verse[cursor + i]);
			line[i].fullRender();
			if(line[i].getImage() != NULL)
			{
				line[i].draw(screen);
			}
		}

		spd.printFPS(screen, "res/Acme/Acme-Regular.ttf",  20, 20);
		wind.draw();
		spd.limitFPS();
		spd.fc++;
	}

	saveSpot(book, cursor);
	saveStates(book, currentFont, fontSize);

	wind.close();

	SDL_Quit();
	return 0;
}



void loadBook(string bookName)
{
	ifstream bookFile(bookName.c_str());
	string temp;
	verse.clear();
	while(bookFile.good())
	{
		getline(bookFile, temp);
		verse.push_back(temp);
	}
	bookFile.close();

}

void saveSpot(string bookName, int readingLine)
{
	string temp = bookName;
	temp += "saveSpot";
	ofstream file(temp.c_str());
	if(file.good())
	{
		file << readingLine;
	}
	file.close();

}

int getSpot(string bookName)
{
	string tempStr = bookName;
	tempStr += "saveSpot";
	ifstream file(tempStr.c_str());
	int temp = 0;
	if(file.good())
	{
		file >> temp;
	}
	file.close();
	return temp;

}

void saveStates(string bookName, int & font, int & fontSize)
{
	ofstream file("LastReadBook.sav");
	file << bookName << " ";
	file << font;
	file << " ";
	file << fontSize;
	file.close();
}



string getStates(int &font, int &fontSize)
{
	string temp = "Directions.txt";
	ifstream file("LastReadBook.sav");
	if(file.good())
	{
		file >> temp;
		file >> font;
		file >> fontSize;
	}
	file.close();
	return temp;


}



void buildDirections()
{
	ofstream file("Directions.txt");
	if(!(file.good()))
	{
		cout << "Read/Write permisions may not be high enough for this file.\n";
	}
	else 
	{
		file << "Failed to\n\n read a bool.\n Good luck.\n";
	}
	file.close();

}
