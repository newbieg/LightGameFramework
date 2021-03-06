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
	this->rect.x = 100;
	this->rect.y = 100;
	this->rect.w = 100;
	this->rect.h = 100;
	this->scaled = false;
}

item::~item()
{

}

item::item(unsigned int color, int x, int y, int w, int h)
{
	this->image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
	SDL_FillRect(this->image, NULL, color);
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = w;
	this->rect.h = h;
	this->needsUpdate = true;
	this->scaled = false;
}

item::item(int x, int y, int w, int h)
{
	this->image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
	SDL_FillRect(this->image, NULL, 0x233397);
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = w;
	this->rect.h = h;
	this->needsUpdate = true;
	this->scaled = false;
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
	if(image != NULL)
	{
		if(scaled)
		{
			SDL_Surface * temp = NULL;
			SDL_BlitScaled(image, NULL, temp, &rect);
		}
		SDL_BlitSurface(image, NULL, dest, &rect);
	}
	else
	{
		cout << "image not found while trying to draw.\n";
	}
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
	this->needsUpdate = true;
	this->image = freshImage;
	this->rect.w = image->clip_rect.w;
	this->rect.h = image->clip_rect.h;
}

SDL_Rect item::update()
{
	return this->getPos();
}

void item::setPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
	this->needsUpdate = true;
}

void item::getPos(int & x, int & y)
{
	x = this->rect.x;
	y = this->rect.y;
}

void item::getImgSize(int & w, int & h)
{
	SDL_Rect temp = this->image->clip_rect;
	w = temp.w;
	h = temp.h;
}

void item::getSize(int & w, int & h)
{
	w = rect.w;
	h = rect.h;
}

void item::setSize(int w, int h)
{
	this->rect.w = w;
	this->rect.h = h;
	this->needsUpdate = true;
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
	this->needsUpdate = true;
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


void item::setDrag(bool beingDragged)
{
	this->dragging = beingDragged;
}

bool item::getDrag()
{
	return this->dragging;
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
	updateRects.push_back(src->getPos());

}

void group::add(group src)
{
	for(int i = 0; i < src.size(); i ++)
	{
		this->add(src.getItem(i));
	}
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
			updateRects.push_back(items[i]->getPos());
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
			updateRects.push_back(items[i]->getPos());
			items[i]->update();
			updateRects.push_back(items[i]->getPos());
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
			updateRects.push_back(items[i]->getPos());
			items[i]->move(x, y);
			updateRects.push_back(items[i]->getPos());
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




vector <SDL_Rect> group::getUpdateRegion()
{
	return this->updateRects;
}

vector <SDL_Rect> group::getRects()
{
	vector <SDL_Rect> allRex;
	for(int i  = 0; i < this->items.size(); i ++)
	{
		allRex.push_back(this->items[i]->getPos());
	}
	return allRex;

}

void group::clearUpdateRegion()
{
	this->updateRects.clear();

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


// return a subset of the items actively being dragged
group group::getDrags()
{
	group subset;
	if(! items.empty())
	{
		for(int i = 0; i < this->items.size(); i ++)
		{
			if(items[i]->getDrag())
			{
				subset.add(items[i]);
			}
		}
	}
	return subset;
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
			this->BTN_State = BTN_HOVER;
			/*
			if(this->hoverConnected == true)
			{
				this->hoverActivated();
			}
			*/
		}
		else if(e->type == SDL_MOUSEBUTTONDOWN)
		{
			this->BTN_State = BTN_DOWN;
		}
		else if(e->type == SDL_MOUSEBUTTONUP)
		{
			this->BTN_State = BTN_CLICK;
		}
		/*
		if(this->connected == true && e->button.clicks == 1)
		{
			activated();
		}
		if(this->dblConnected == true && e->button.clicks >= 2)
		{
			dblActivated();
		}
		if(this->stateImg[BTN_State] != NULL)
		{
			this->image = this->stateImg[BTN_State];
		}
		*/
		if(this->stateImg[BTN_State] != NULL)
		{
			this->image = this->stateImg[BTN_State];
		}
		return true;

	}
	BTN_State = BTN_UP;
	if(this->stateImg[BTN_State] != NULL)
	{
		this->image = this->stateImg[BTN_State];
	}
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

void button::onHover(void(*function)())
{
	hoverActivated = function;
	hoverConnected = true;
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



void speedLimit(const int fps, int &startTime)
{
	int current = SDL_GetTicks();
        if((1000/fps) > current - startTime)
        {
                SDL_Delay(1000/fps - (current - startTime));
        }
	startTime = SDL_GetTicks();
}


SDL_Surface * optLoad(string path, const SDL_Surface* dest )
{
	cout << "Loading Image";
        SDL_Surface * opt = NULL;
        SDL_Surface* loader = IMG_Load(path.c_str());
        opt = SDL_ConvertSurface(loader, dest->format, 0);
	SDL_FreeSurface(loader);
        return opt;
}




