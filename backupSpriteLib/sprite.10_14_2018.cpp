#include "sprite.h"
#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>



using namespace std;





//===============   Item =======//




item::item()
{

}

item::~item()
{

}

item::item(unsigned int color, int x, int y, int w, int h)
{
	this->image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
	SDL_FillRect(this->image, NULL, color);
	this->rect = this->image->clip_rect;
	this->rect.x = x;
	this->rect.y = y;

}

item::item(int x, int y, int w, int h)
{
	this->image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
	SDL_FillRect(this->image, NULL, 0x233397);
	this->rect = this->image->clip_rect;
	this->rect.x = x;
	this->rect.y = y;

}

unsigned int item::setColor(int r, int g, int b)
{
	// this might be overly hopefull that image matches screen format...
	unsigned int color = SDL_MapRGB(this->image->format, r, g, b);
	SDL_FillRect(this->image, NULL, color);
	return color;
}

void item::draw(SDL_Surface* dest)
{
	SDL_BlitSurface(image, NULL, dest, &rect);
}

bool item::operator==(const item &other) const
{
	return (this->image == other.getImage());
}

SDL_Surface * item::getImage() const
{
	return this->image;
}

void item::setImage(SDL_Surface * freshImage)
{
	this->image = freshImage;
}

void item::update()
{

}

void item::setPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void item::getPos(int & x, int & y)
{
	x = this->rect.x;
	y = this->rect.y;
}

void item::getSize(int & x, int & y)
{
	SDL_Rect temp = this->image->clip_rect;
	x = temp.w;
	y = temp.h;
}

void item::setSize(int w, int h)
{
	this->rect.w = w;
	this->rect.h = h;
}

SDL_Rect item::getPos()
{
	return this->rect;
}

bool item::checkCollision(item & other)
{
	if(this->getPos().x < other.getPos().x + other.getPos().w && 
	this->getPos().x + this->getPos().w > other.getPos().x &&
	this->getPos().y < other.getPos().y + other.getPos().h && 
	this->getPos().y + this->getPos().h > other.getPos().y)
	{
		return true;
	}
	return false;
}

bool item::isInside(item & other)
{
	if(this->getPos().x > other.getPos().x &&
	this->getPos().x + this->getPos().w < other.getPos().x + other.getPos().w &&
	this->getPos().y > other.getPos().y &&
	this->getPos().y + this->getPos().h < other.getPos().y + other.getPos().h)
	{
		return true;
	}
	return false;
}

bool item::isInside(int x, int y)
{
	if(x > this->getPos().x && x < this->getPos().x + this->getPos().w)
	{
		if(y > this->getPos().y && y < this->getPos().y + this->getPos().h)
		{
			return true;
		}
	}
	return false;
}

int item::getCenterX()
{
	return (this->rect.x + (this->rect.w/2));
}
int item::getCenterY()
{
	return (this->rect.y + (this->rect.h/2));
}

void item::move(int x, int y)
{
	this->rect.x += x;
	this->rect.y += y;
}

void item::save(ofstream & file)
{
	file << rect.x << " ";
	file << rect.y << " ";
	file << endl;
}

void item::load(ifstream & file)
{
	file >> rect.x;
	file >> rect.y;
}







//======================= group =======//



group group::copy()
{
	group newGroup;
	for(int i = 0; i < this->items.size(); i ++)
	{
		newGroup.add(items[i]);
	}
	return newGroup;
}

void group::add(item * src)
{
	this->items.push_back(src);
}

int group::size()
{
	return this->items.size();
}

void group::remove(item desc)
{
	for(int i = 0; i < items.size(); i ++)
	{
		if(desc == *items[i])
		{
			items.erase(items.begin() + i);
		}
	}
}

bool group::has(item desc)
{
	if(this->isEmpty())
	{
		return false;
	}
	for(int i = 0; i < items.size(); i ++)
	{
		if(*items[i] == desc)
		{
			return true;
		}
	}
	return false;
}

void group::update()
{
	if(! items.empty())
	{
		for(int i = 0; i < this->items.size(); i ++)
		{
			items[i]->update();
		}
	}
}

void group::draw(SDL_Surface *dest)
{
	if(! items.empty())
	{
		for(int i = 0; i < this->items.size(); i ++)
		{
			items[i]->draw(dest);
		}
	}
}

void group::clear()
{
	items.clear();
}

bool group::isEmpty()
{
	return this->items.empty();
}

void group::move(int x, int y)
{
	if(! items.empty())
	{
		for(int i = 0; i < this->items.size(); i ++)
		{
			items[i]->move(x, y);
		}
	}
}

void group::load(string fileName)
{
	ifstream inFile(fileName.c_str());
	if(! items.empty() && inFile.good())
	{
		item temp;
		this->add(&temp);
		for(int i = 0; i < this->items.size(); i ++)
		{
			items[i]->load(inFile);
		}
	}
	inFile.close();
}

void group::save(string fileName)
{

	ofstream outFile(fileName.c_str());
	if(! items.empty() && outFile.good())
	{
		for(int i = 0; i < this->items.size(); i ++)
		{
			items[i]->save(outFile);
		}
	}
	outFile.close();
}



// return a subset of items that the colider has colided with.
// a subset will allow for each item to be dealt with in case they
// are stacked on a tile...
group group::getColision(item& colider)
{
	group subset;
	if(! items.empty())
	{
		for(int i = 0; i < this->items.size(); i ++)
		{
			if(items[i]->checkCollision(colider))
			{
				subset.add(items[i]);
			}
		}
	}
	return subset;
}

group group::getClicked(int x, int y)
{
	group subset;
	if(! items.empty())
	{
		for(int i = 0; i < this->items.size(); i ++)
		{
			if(items[i]->isInside(x, y))
			{
				subset.add(items[i]);
			}
		}
	}
	return subset;
}

item* group::getItem(int index)
{
	return items[index];
}

vector <item*> group::getItems()
{
	return items;
}








// Child Classes below this point;
//
// -------------------------------









/*
// ============= Animation ===============
void addImage(std::string pathToImage)
{
	SDL_Surface tempIm = IMG_Load(pathToImage.c_str());
	images.push_back(tempIm);
}
void draw(SDL_Surface *dest)
{

	for(int i = 0; i < images.size(); i ++)
	{
		SDL_BlitSurface(images[i], NULL, dest, &this->rect);
	}
}

void remove(int index); // remove an image at index
void clear(); // clear the entire animation
bool isEmpty(); // check if animation is empty
void setFrameRate(int frameRateSet); // change animation speed

//        protected:
//          std::vector <SDL_Surface*> images;
//          int frameRate;
//          int lastFrame;


*/





// ===============  txt ================
// A text rendering class


txt::txt()
{
	size = 14;
	this->color = {0,0,0};
}

txt::txt(std::string text, int x, int y)
{

	words = text;
	size = 14;
	this->color = {0,0,0};
	this->rect.x = x;
	this->rect.y = y;
	this->font = TTF_OpenFont("res/Acme/Acme-Regular.ttf", size);
	this->image = TTF_RenderText_Solid(font, text.c_str(), color);
	TTF_CloseFont(this->font);
}

void txt::setTxt(std::string text)
{
	words = text;
	this->image = TTF_RenderText_Solid(font, words.c_str(), color);
}

void txt::setFontSize(int size)
{
	this->size = size;
	this->font = TTF_OpenFont("res/Acme/Acme-Regular.ttf", size);
	this->image = TTF_RenderText_Solid(font, words.c_str(), color);

}

void txt::setColors(unsigned int foreground, unsigned int background)
{

}

void txt::setFont(string path)
{
	this->font = TTF_OpenFont(path.c_str(), size);
//	this->font = TTF_OpenFont("res/Acme/Acme-Regular.ttf", size);
	this->image = TTF_RenderText_Solid(font, words.c_str(), color);
	
}








//======================== BUTTON CLASS




button::button()
{
	this->image = SDL_CreateRGBSurface(0, 30, 30, 32, 0,0,0,0);
	SDL_FillRect(this->image, NULL, SDL_MapRGB(this->image->format, 30, 200, 30));
	stateImg.push_back(this->image); // up
	stateImg.push_back(this->image); // hover
	stateImg.push_back(this->image); // down
	stateImg.push_back(this->image); // click
	stateImg.push_back(this->image); // dbl click
	this->connected = false;
	this->dblConnected = false;

}

void button::setImage(int btnEnum, SDL_Surface* theImage)
{
	stateImg[btnEnum] = theImage;

}

bool button::eventCheck(SDL_Event * e)
{
	if(this->isInside(e->button.x, e->button.y) || this->isInside(e->motion.x, e->motion.y))
	{
		if(e->type == SDL_MOUSEMOTION )
		{
			this->image = this->stateImg[BTN_HOVER];
			this->BTN_State = BTN_HOVER;
		}
		else if(e->type == SDL_MOUSEBUTTONDOWN)
		{
			this->image = this->stateImg[BTN_DOWN];
			this->BTN_State = BTN_DOWN;
		}
		else if(e->type == SDL_MOUSEBUTTONUP)
		{
			this->image = this->stateImg[BTN_UP];
			this->BTN_State = BTN_UP;
		}
		if(this->connected == true && e->button.clicks == 1)
		{
			activated();
		}
		if(this->dblConnected == true && e->button.clicks >= 2)
		{
			dblActivated();
		}
		return true;

	}
	this->image = this->stateImg[BTN_UP];
	BTN_State = BTN_UP;
	return false;
}

int button::getState()
{
	return this->BTN_State;
}

void button::onClick(void (*function)())
{

	activated = function;
	connected = true;
}

void button::onDblClick(void (*function)())
{

	dblActivated = function;
	dblConnected = true;
}







// ==================== Extraneous Functions




int getSign(int number)
{
	if(number > 0)
	       return 1;
	else if(number < 0)
		return -1;
	else
		return 0;	
}



void speedLimit(const int fps, const int startTime)
{
        if((1000/fps) > SDL_GetTicks() - startTime)
        {
                SDL_Delay(1000/fps - (SDL_GetTicks() - startTime));
        }
}


SDL_Surface * optLoad(string path, const SDL_Surface* dest )
{
        SDL_Surface * opt = NULL;
        SDL_Surface* loader = IMG_Load(path.c_str());
        opt = SDL_ConvertSurface(loader, dest->format, 0);
        return opt;

}



