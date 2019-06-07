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
	int readSpeed = 60; // words per minute
	string fileName;
	for(int i = 1; i < argc; i ++)
	{
		if(strcmp(argv[i], "-speed") == 0)	
		{
			i ++;
			readSpeed = atoi(argv[i]);
		}
		else if(strcmp(argv[i], "-f") == 0)
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
	window wind;
	wind.show();


	closeFramework();

}
