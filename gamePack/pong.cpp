#include <SDL2/SDL.h>
#include "../sprite.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>



SDL_Window* wind;
const int windX = 500;
const int windY = 400;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;
const int paddleW = 10;
const int paddleL = 80;

int gameLoop(int playerDir);



int startTime;


using namespace std;

int main(int argc, char ** argv)
{
	srand(time(NULL));
	int gameCount = 10;


	SDL_Init(SDL_INIT_EVERYTHING);
	wind = SDL_CreateWindow("Title",SDL_WU, SDL_WU , windX, windY, SDL_WINDOW_OPENGL);
	if(wind == NULL)
	{
		cout << "Could not create Window\n";
	}
	if(argc == 2)
	{
		stringstream ss;
		ss << argv[1];
		ss >>gameCount;
	}
	cout << "Welcome to " << argv[0] << "\nSCORES BELOW:\n";



	int winner = 0;
	int score1 = 0;
	int score2 = 0;

	for(int i = 0; i < gameCount; i ++)
	{
		cout << "Player 1: " << score1 << "    Player 2: " << score2 << endl;
		winner = gameLoop((score1>score2)? -1 : 1);
		if(winner > 0)
		{
			score1 ++;
		}
		else if(winner < 0)
		{
			score2 ++;
		}
		else
		{
			exit(0);
		}
	}
	if(score1 == score2)
	{
		cout << "CHALLENGE ROUND!\n";
		winner = gameLoop((rand()%2>0)? 1 : -1);
		if(winner > 0)
		{
			score1 ++;
		}
		else if(winner < 0)
		{
			score2 ++;
		}
	}


	cout << "\n\nFINAL SCORE: \n";
	cout << "Player 1: " << score1 << "    Player 2: " << score2 << endl;

	SDL_DestroyWindow(wind);

	SDL_Quit();
	return 0;
}





int gameLoop(int playerDir)
{
	int paddleSpeed = 2;
	SDL_Surface* screen = SDL_GetWindowSurface(wind);
	SDL_Event events;
	unsigned int white = SDL_MapRGB(screen->format,255,255,255);
	unsigned int offWhite = SDL_MapRGB(screen->format,155,255,255);
	unsigned int red = SDL_MapRGB(screen->format,255,10,10);
	unsigned int blue = SDL_MapRGB(screen->format,10,10,255);
	SDL_FillRect(screen, NULL, white); 
	item paddle1(red, 4, 40, paddleW, paddleL);
	item paddle2(red, (windX - paddleW) - 5, 40, paddleW, paddleL);
	item ball(blue, (windX - 30)/2, (windY - 30)/2, 30, 30);
	item boarder(offWhite, 2,0 + 31, (windX  - 4) , windY - 62);
	group activeItems;
	activeItems.add(&boarder);
	activeItems.add(&paddle1);
	activeItems.add(&paddle2);
	activeItems.add(&ball);


	bool paddle1_up = false;
	bool paddle1_down = false;
	bool paddle2_up = false;
	bool paddle2_down = false;
	double ballSpeedX = -2 * playerDir;
	double ballSpeedY = -1;
	int turn = 1;
	cout << "StartSpeedX: " << ballSpeedX << endl;
	cout << "paddle2 x: " << paddle2.getPos().x;
	cout << "y: " << paddle2.getPos().y << endl;

	vector <SDL_Rect> regions;

	SDL_UpdateWindowSurface(wind);
	while(true)
	{
		startTime = SDL_GetTicks();
		while(SDL_PollEvent(&events))
		{
			if(events.type == SDL_QUIT)
			{
				return 0;
			}
			else if(events.type == SDL_KEYDOWN)
			{
				if(events.key.keysym.sym == SDLK_UP)
				{
					paddle2_up = true;
				}
				else if(events.key.keysym.sym == SDLK_DOWN)
				{
					paddle2_down = true;
				}
				if(events.key.keysym.sym == SDLK_w)
				{
					paddle1_up = true;
				}
				else if(events.key.keysym.sym == SDLK_s)
				{
					paddle1_down = true;
				}
			}
			else if(events.type == SDL_KEYUP)
			{
				if(events.key.keysym.sym == SDLK_UP)
				{
					paddle2_up = false;
				}
				else if(events.key.keysym.sym == SDLK_DOWN)
				{
					paddle2_down = false;
				}
				if(events.key.keysym.sym == SDLK_w)
				{
					paddle1_up = false;
				}
				else if(events.key.keysym.sym == SDLK_s)
				{
					paddle1_down = false;
				}
			}
		}

		int x1, y1, x2, y2, ballx, bally;
		ball.getPos(ballx, bally);
		paddle1.getPos(x1, y1);
		paddle2.getPos(x2, y2);


		bool coll1 = ball.checkCollision(paddle1);
		bool coll2 = ball.checkCollision(paddle2);
		if(coll1)
		{
			turn ++;
			ballSpeedX *= -1;
			if(!(turn % 6) && ballSpeedX < 15)
			{
				ballSpeedX ++;
			}
			ballSpeedY -= (paddle1.getCenterY() - ball.getCenterY())/10;
		}
		else if(coll2)
		{
			turn ++;
			ballSpeedX *= -1;
			if(!(turn % 6))
			{
				ballSpeedX --;
			}
			ballSpeedY -= (paddle2.getCenterY() - ball.getCenterY())/10;
		}
		if(! ball.isInside(boarder))
		{
			SDL_Rect ballr = ball.getPos();
			SDL_Rect boardr = boarder.getPos();
			if(ballr.x < boardr.x || ballr.x + ballr.w > boardr.x + boardr.w)
			{
				return (int) ballSpeedX;
			}
			else
			{
				ballSpeedY *= -1;
			}
		}

		paddle1.setPos(x1, y1 +(paddle1_down - paddle1_up) * paddleSpeed);
		paddle2.setPos(x2, y2 +(paddle2_down - paddle2_up) * paddleSpeed);
		ball.setPos(ballx + ballSpeedX, bally + ballSpeedY);
		
		SDL_FillRect(screen, NULL, white); 
		boarder.draw(screen);
		activeItems.update();
		activeItems.draw(screen);

		regions = activeItems.getUpdateRegion();
		
		SDL_UpdateWindowSurfaceRects(wind, &regions[0], regions.size());
		activeItems.clearUpdateRegion();		

		speedLimit(60, startTime);
	}
	SDL_FreeSurface(screen);
}


