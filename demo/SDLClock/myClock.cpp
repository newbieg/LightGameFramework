#include "sprite.h"
#include <iostream>
#include <string>
#include <time.h>

SDL_Window* wind;
const int windX = 550;
const int windY = 500;
const int SDL_WU = SDL_WINDOWPOS_UNDEFINED;
std::string basepath;


using namespace std;

string intToStr(int value);
string timerToStr(time_t timer);

void changeTimePopup(time_t &offerTimeChange, string fontAddress);

int main(int argc, char ** argv)
{
	initFramework();
	basepath = SDL_GetPrefPath("Softfoot_Software", "SDL_Pompoko");
	sound tom(basepath +"res/tone.wav");
	time_t defaultTimer = 20 * 60;
	string fontPath = basepath + "res/Acme/Acme-Regular.ttf";
	
	window wind("Template Window", windX, windY, SDL_WINDOW_RESIZABLE);
	SDL_Surface* screen;
	wind.linkScreen(&screen);

	animation spinner;
	for(int i = 1; i < 18; i ++)
	{
		string path = basepath + "res/rotate/00";
		if(i < 10)
		{
			path += "0";
		}
		path += intToStr(i);
		path += ".png";
		spinner.addImage(path);
	}

	textInput txt_input("Purpose: ", fontPath, 60,0);
	txt txt_clock("1200", fontPath, 15,0);
	txt_clock.setFontSize(180);
	txt_clock.fullRender();
	txt txt_start("Restart", fontPath, 0,0);
	txt_start.setFontSize(100);
	txt_start.fullRender();
	button start;
	start.setImage(BTN_UP, basepath +"res/up.png");
	start.setImage(BTN_HOVER, basepath +"res/hover.png");
	start.setImage(BTN_DOWN, basepath +"res/down.png");
	start.setImage(BTN_CLICK, start.getImage(BTN_UP));
	txt_start.stamp(start.getImage(BTN_UP), 20, 15);
	txt_start.stamp(start.getImage(BTN_HOVER), 20, 15);
	txt_start.stamp(start.getImage(BTN_DOWN), 20, 15);

	start.setPos(90, 250);
	group clickables;
	clickables.add(&spinner);

	
	bool run = true;
	speed spd;
	spd.fps = 10;
	spd.redux = SDL_GetTicks();
	time_t startTimer = time(NULL);
	time_t timer = defaultTimer;
	bool alarm = false;
	while(run)
	{
	
		spinner.next();
		SDL_Event ev;
		while(SDL_PollEvent(&ev) != 0)
		{
			run = wind.handleEvent(ev);
			txt_input.handleEvent(ev);
			if(ev.type == SDL_QUIT)
			{
				run = false;
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
					case SDLK_SPACE:
					break;
				}
			}
			else if(ev.type == SDL_MOUSEBUTTONUP)
			{
				if(clickables.getClicked(ev.button.x, ev.button.y).has(spinner))
				{
					changeTimePopup(defaultTimer, fontPath);
					startTimer = time(NULL);
				}
			}
			if(start.eventCheck(ev))
			{
				if(start.getState() == BTN_CLICK)
				{
					startTimer = time(NULL);
					alarm = false;
				}
			}
		}
		
		timer = defaultTimer - (time(NULL) - startTimer);
		if(timer <= 0)
		{
			timer = 0;
			if(! alarm)
			{
				alarm = true;
				tom.play();
			}
		}
		// Draw to screen here.
		SDL_FillRect(screen, NULL, 0xffffff);
		start.draw(screen);
		txt_clock.setText(timerToStr(timer));
		txt_clock.fullRender();
		txt_clock.draw(screen);
		txt_input.draw(screen);
		spinner.draw(screen);
//		spd.printFPS(screen, "res/Acme/Acme-Regular.ttf",  20, 20);
		wind.draw();
		spd.limitFPS();
		spd.fc++;
	}
	wind.close();
	closeFramework();
	return 0;
}

string intToStr(int value)
{
	if(value == 0)
	{
		return "0";
	}
	string flipped, outp;
	while(value != 0)
	{
		char num = ((char) (value % 10));
		char tmp = '0' + num;
		flipped += tmp;
		value -= num;
		value = value / 10;
	}
	for(int i = flipped.length() - 1; i >= 0; i --)
	{
		outp += flipped[i];
	}

	return outp;
}

string timerToStr(time_t timer)
{
	if(timer == 0)
	{
		return "0:00:00";
	}
	string outp;

	int hr = timer / 3600;
	timer -= hr * timer;
	int min = timer / 60;
	int sec = timer % 60;
	if(hr > 0)
	{
		while(hr > 9)
		{
			outp += hr / 10 + '0';
			hr /= 10;
		}
		outp += hr % 10 + '0';
		outp += ':';
	}
	outp += min / 10 + '0';
	outp += min % 10 + '0';
	outp += ':';
	outp += sec / 10 + '0';
	outp += sec % 10 + '0';
	
	return outp;
}

void changeTimePopup(time_t &offerTimeChange, string fontAddress)
{
	window wind("Change Time", 400, windY/4, SDL_WINDOW_RESIZABLE);
	group clickables;
	txt number(timerToStr(offerTimeChange), fontAddress, 40, 6);
	slider testSlider;
	testSlider.setSize(380, 20);
	testSlider.setPos(10, 30);
	testSlider.setMin(600);
	testSlider.setMax(3600);
	testSlider.setValue((double)(offerTimeChange));
	button *mid = testSlider.getMiddleButton();
	mid->setImage(BTN_UP, basepath+"res/smile.png");
	mid->setImage(BTN_DOWN, basepath+"res/smile.png");
	mid->setImage(BTN_CLICK, basepath+"res/smile.png");
	mid->setImage(BTN_HOVER, basepath+"res/smile.png");

	speed spd;
	spd.fps = 20;
	spd.redux = SDL_GetTicks();
	bool run = true;
	while(run)
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev)!= 0)
		{
			run = wind.handleEvent(ev);
			if(testSlider.handleEvent(ev))
			{
				number.setText(timerToStr(testSlider.getValue()));
				number.fullRender();
			}
		}
		SDL_FillRect(wind.getImage(), NULL, 0xffffff);
		number.draw(wind.getImage());
		spd.printFPS(wind.getImage(), fontAddress,  20, 60);
		testSlider.draw(wind.getImage());
		wind.draw();
		spd.limitFPS();
		spd.fc++;
	}
	offerTimeChange = testSlider.getValue();
//	number.free();
	testSlider.free();
	wind.close();
	return;
}
