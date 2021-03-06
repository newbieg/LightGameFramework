#include "sprite.h"
#include <SDL2/SDL.h>
#include <png.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cstdio>


using namespace std;





//===============   Item =======//

void oshit()
{
	SDL_Quit();
	exit(0);
}

void initFramework()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	// to build the framework, use the following compile command:
	// g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lpng yourFile.cpp sprite.cpp 	
}

void scanLineDraw(SDL_Surface * dest, unsigned int color, int x, int y, int x2)
{
	for(int i = x; i < x2; i ++)
	{
		putPixel(dest, i, y, color);
	}
}

void freeVector(vector<SDL_Surface*> &vec)
{
	for(int i = 0; i < vec.size(); i ++)
	{
		if(vec[i] != NULL)
		{
			SDL_FreeSurface(vec[i]);
			vec[i] = NULL;
		}
	}
}


unsigned long item::itemCount = 0;
int tile::TileWidth = 0;
int tile::TileHeight = 0;


item::item(const item &copy)
{
	this->rect.x = copy.rect.x;
	this->rect.y = copy.rect.y;
	this->rect.w = copy.rect.w;
	this->rect.h = copy.rect.h;
	this->image = copy.image;
	this->scaled = copy.scaled;
	this->dragging = copy.dragging;
	this->id = copy.id;
	this->safeTrackImage = copy.image;

}

item::item(SDL_Rect size)
{
	this->rect.x = size.x;
	this->rect.y = size.y;
	this->rect.w = size.w;
	this->rect.h = size.h;
	this->scaled = false;
	itemCount ++;
	this->id = item::itemCount;
	this->safeTrackImage = NULL;
	this->image = NULL;

}

item::item()
{
	this->rect.x = 0;
	this->rect.y = 0;
	this->rect.w = 0;
	this->rect.h = 0;
	this->scaled = false;
	itemCount ++;
	this->id = item::itemCount;
	this->safeTrackImage = NULL;
	this->image = NULL;
}

item::~item()
{
	// no guarantee that the image used here is
	// not also in use or already freed
	// ... hoping for a better solution.
	//
	// if a resource is truly not needed, use free()
}


// all children classes which also need to free resources should
// override this function and then call this parent function
void item::free()
{
	if(this->safeTrackImage != NULL && !(this->image == this->safeTrackImage))
	{
		SDL_FreeSurface(this->safeTrackImage);
		this->safeTrackImage = NULL;
	}
	if(this->image != NULL)
	{
		SDL_FreeSurface(this->image);
		this->image = NULL;
	}
}


item::item(unsigned int color, int x, int y, int w, int h)
{
	this->image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
	this->safeTrackImage = this->image;
	SDL_FillRect(this->image, NULL, color);
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = w;
	this->rect.h = h;
	this->needsUpdate = true;
	this->scaled = false;
	itemCount ++;
	this->id = item::itemCount;
}

item::item(int x, int y, int w, int h)
{
	this->image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
	this->safeTrackImage = this->image;
	SDL_FillRect(this->image, NULL, 0x3c392f);
	this->rect.x = x;
	this->rect.y = y;
	this->rect.w = w;
	this->rect.h = h;
	this->needsUpdate = true;
	this->scaled = false;
	itemCount ++;
	this->id = item::itemCount;
}

unsigned int item::setColor(int r, int g, int b)
{
	this->color = SDL_MapRGB(this->image->format, r, g, b);
	SDL_FillRect(this->image, NULL, this->color);
	return color;
}

unsigned int item::setColor(int r, int g, int b, int a)
{
	this->color = SDL_MapRGBA(this->image->format, r, g, b, a);
	SDL_FillRect(this->image, NULL, this->color);
	return color;
}

unsigned int item::setColor(unsigned int mapColor)
{
	SDL_FillRect(this->image, NULL, mapColor);
	this->color = mapColor;
	return mapColor;
}



unsigned long item::getID() const
{
	return this->id;
}

unsigned long item::getItemCount()
{
	return itemCount;
}

void item::draw(SDL_Surface* dest)
{
	if(image != NULL)
	{
		if(scaled)
		{
			SDL_BlitScaled(image, NULL, dest, &rect);
		}
		else
		{
			SDL_BlitSurface(image, NULL, dest, &rect);
		}
	}
	else
	{
		cout << "item id =  " << this->getID() << "Had an empty image.\n";
	}
}

void item::stamp(SDL_Surface * dest, int x, int y)
{
	if(image != NULL)
	{
		SDL_Rect temp = this->rect;
		temp.x = x;
		temp.y = y;
		SDL_BlitSurface(this->image, NULL, dest, &temp);
	}
	else
	{
		cout << "image not found while trying to stamp.\n";
	}
}
void item::stamp(SDL_Surface * dest, SDL_Rect sub, int x, int y)
{
	if(image != NULL)
	{
		SDL_Rect temp;
		temp.x = x;
		temp.y = y;
		SDL_BlitSurface(this->image, &sub, dest, &temp);
	}
	else
	{
		cout << "image not found while trying to draw.\n";
	}
}

bool item::operator==(const item &other) const
{
	return (this->id == other.getID());
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

void item::setImage(string path)
{
	this->needsUpdate = true;
	this->image = IMG_Load(path.c_str());
	this->rect.w = image->clip_rect.w;
	this->rect.h = image->clip_rect.h;
}


void item::addColor(const unsigned int r, const unsigned int g, const unsigned int b)
{
	unsigned int tx = 0;
	unsigned int ty = 0;
	unsigned char pr, pg, pb;
	unsigned int leng = this->rect.w * this->rect.h;
	unsigned int * ptr;
	for(ptr = (unsigned int *) this->image->pixels; ptr < ((unsigned int * )this->image->pixels) +leng; ptr ++)
	{
		SDL_GetRGB(*ptr, this->image->format, &pr, &pg, &pb);
	       	pr += r;
		pg += g;
		pb += b;
		unsigned int color = SDL_MapRGB(this->image->format, pr, pg, pb);
		*ptr = color;
	}
}
void item::addColor(bool maxOut, const unsigned int r, const unsigned int g, const unsigned int b)
{
	unsigned int tx = 0;
	unsigned int ty = 0;
	unsigned char pr, pg, pb;
	unsigned int leng = this->rect.w * this->rect.h;
	unsigned int * ptr;
	for(ptr = (unsigned int *) this->image->pixels; ptr < ((unsigned int * )this->image->pixels) +leng; ptr ++)
	{
		unsigned int tr = r, tg = g, tb = b;
		SDL_GetRGB(*ptr, this->image->format, &pr, &pg, &pb);
	       	tr += pr;
		tg += pg;
		tb += pb;
		if(maxOut)
		{
			if(tr > 255)
				tr = 255;
			if(tb > 255)
				tb = 255;
			if(tg > 255)
				tg = 255;
		}
		pr = tr;
		pg = tg;
		pb = tb;

		unsigned int color = SDL_MapRGB(this->image->format, pr, pg, pb);
		*ptr = color;
	}
}


void item::averageImage(SDL_Surface* other)
{
	int width = this->rect.w;
	int skipW = 0;
	if(other->pitch > this->image->pitch)
	{
		skipW = (other->pitch - this->image->pitch) / 4;
	}
	for(int i = 0; i < this->rect.w; i ++)
	{
		cout << "I'll leave this unimplemted for now,\nNot the highest priority in gaming right now. might be usefull in creating\n an\"Under\" type animation when user goes behind a building or something.\n";
	}

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

void item::setRect(SDL_Rect r)
{
	this->rect = r;
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
	if(this->image != NULL)
	{
		SDL_FreeSurface(this->image);
	}
	this->image = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);
	setColor(this->color);
}

SDL_Rect item::getPos() const
{
	return this->rect;
}

bool item::checkCollision(const item & other) const
{
	SDL_Rect temp = other.getPos();
	return this->checkCollision(temp);
}
bool item::checkCollision(const SDL_Rect & bdr) const
{
	if(this->getPos().x < bdr.x + bdr.w && 
	this->getPos().x + bdr.w > bdr.x &&
	this->getPos().y < bdr.y + bdr.h && 
	this->getPos().y + this->getPos().h > bdr.y)
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



/*
// RECORDING AN ITEM

void item::saveRecording(ofstream &file)
{}
void item::loadRecording(ifstream &file)
{}
void item::playRecording()
{}
void item::record(bool state)
{
	recordingState = state;
}
void item::recordAutoSave(ofstream &file, int time)
{}


bool item::recordKey(int time)
{
	if(recordingState)
	{
		recX.push_back(this->rect.x);
		recY.push_back(this->rect.x);
		recKey.push_back(time);
		if(!(time %recTFrames))
		{
			//
		}
	}
	return recordingState;
}
*/


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

void group::free()
{
	for(int i = 0; i < items.size(); i ++)
	{
		items[i]->free();
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
group group::getCollision(item& colider)
{
	group subset;
	if(!(items.empty()))
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




//======================== WINDOW CLASS =================

window::window()
{
	
	windowName = "No Title";
	wind = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 150, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
	if(wind == NULL)
	{
		cout << "Failed to create Window " << windowName;
		return;
	}
	this->image = SDL_GetWindowSurface(wind);
	SDL_UpdateWindowSurface(wind);
}

window::window(string title, int width, int height)
{
	windowName = title;
	wind = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if(wind == NULL)
	{
		cout << "Failed to create Window " << title;
		return;
	}
	this->image = SDL_GetWindowSurface(wind);
	SDL_UpdateWindowSurface(wind);

}

void window::setTitle(string title)
{
	windowName = title;
	SDL_SetWindowTitle(wind, title.c_str());
}

string window::getTitle()
{
	return SDL_GetWindowTitle(wind);
}

SDL_Rect window::update()
{
	SDL_UpdateWindowSurface(this->wind);
	return this->rect;
}

void window::hide()
{
	SDL_HideWindow(wind);
}

void window::show()
{
	SDL_ShowWindow(wind);
}

void window::close()
{
	SDL_FreeSurface(this->image);
	SDL_DestroyWindow(this->wind);
}






//======================== TILE CLASS =================
void tile::setState()
{
	
}

void tile::add(item layer)
{
	layers.push_back(layer);
}

void tile::free()
{
	for(int i = 0; i < layers.size(); i ++)
	{
		layers[i].free();
	}
	item::free();
}

void tile::swapLayers(int first, int second)
{
	item temp = layers[first];
	layers[first] = layers[second];
	layers[second] = temp;
}

void tile::draw(SDL_Surface* dest)
{
	for(int i = 0; i < layers.size(); i ++)
	{
		layers[i].draw(this->image);
	}
	if(this->image != NULL)
	{
		SDL_BlitSurface(this->image, NULL, dest, &rect);
	}
}
void tile::stamp(SDL_Surface* dest, int x, int y)
{
	SDL_Rect temp = this->rect;
	temp.x = x;
	temp.y = y;
	for(int i = 0; i < layers.size(); i ++)
	{
		layers[i].draw(this->image);
	}
	if(this->image != NULL)
	{
		SDL_BlitSurface(this->image, NULL, dest, &temp);
	}
}


void tile::drawLayer(SDL_Surface * dest, int depth)
{
	layers[depth].draw(dest);
}


// ==========  BOARD ===============

board::board()
{

}

void board::setDimensions(int w, int h)
{
	this->tw = w;
	this->th = h;
	this->padding.x = 0;
	this->padding.y = 0;
	this->padding.w = 640;
	this->padding.h = 480;
	this->image = SDL_CreateRGBSurface(0, w * TILESIZE, h * TILESIZE, 32, 0,0,0,0);
	this->rect = this->image->clip_rect;
}

void board::setCamera(SDL_Rect windowSize)
{
	this->camera = windowSize;
}

void board::setCamera(int x, int y, int w, int h)
{
	this->camera.x = x;
	this->camera.y = y;
	this->camera.w = w;
	this->camera.h = h;
	this->padding.w = w;
	this->padding.h = h;
}

void board::draw(SDL_Surface* dest)
{
	if(this->image != NULL)
	{
		SDL_BlitSurface(this->image, &this->camera, dest, NULL);
	}
}

void board::drawDriven(SDL_Surface * dest, int frameCount)
{
	
}

void board::addTile(item toAdd, int x, int y)
{
	toAdd.stamp(this->image, x*TILESIZE, y*TILESIZE);
}

void board::addTile(item toAdd, int x, int y, int subx, int suby)
{
	toAdd.stamp(this->image, x*TILESIZE+subx, y*TILESIZE+suby);
}


void board::fillBoard(item toUse)
{
	if(toUse.getImage() != NULL)
	{
		int tempw = toUse.getPos().w;
		int temph = toUse.getPos().h;
		for(int i = 0; i*tempw < tw*32; i ++)
		{
			for(int j = 0; j*temph < th*32; j ++)
			{
				toUse.stamp(this->image, i*tempw +rect.x, j*temph + rect.y);
			}
		}
	}
	else
	{
		cout << "item.id = " << toUse.getID() << "had an empty image\n";
	}
}

SDL_Rect* board::getCamera()
{
	return &this->camera;
}

void board::setPadding(int x, int y, int w, int h)
{
	this->padding.x = x;
	this->padding.y = y;
	this->padding.w = w;
	this->padding.h = h;
}

// We'll have to work on this...
void board::move(int x, int y)
{
	item temp;
	int centerX = camera.w/2;
	int centerY = camera.h/2;
	if(!(walk.isEmpty()))
	{
		temp.setRect(walk.getItem(0)->getPos());
		temp.move(x, y);

		// Check if we can move to the new location
		if(solids.getCollision(temp).isEmpty())
		{

			if(camera.x + x > padding.x && camera.x + x < this->rect.w - padding.w && (temp.getPos().x > centerX - 4 && temp.getPos().x < centerX + 4))
			{
				camera.x += x;
			}
			else if(temp.getPos().x + temp.getPos().w + 2 < camera.w)
			{
				walk.move(x, 0);
			}	
			else if(camera.x + x < this->rect.w - padding.w)
			{
				camera.x += x;
			}
			if(camera.y + y > padding.y && camera.y + y < this->rect.h - padding.h && (temp.getPos().y > centerY - 4 && temp.getPos().y < centerY + 4))
			{
				camera.y += y;
			}
			else if(temp.getPos().y + temp.getPos().h + 2 < camera.h)
			{
				walk.move(0, y);
			}
			else if(camera.y + y < this->rect.h - padding.h || camera.y + y > padding.y) // I have a logic problem to work out here.
		// the origin if needs nesting rather than this finale else if...
//		I left off here
			{
				camera.y += y;
			}
		}
	}
}


// ============= Animation ===============

animation::animation()
{
	step = 1;
	limitter = 2;
	skip = 1;
}
void animation::free()
{
	freeVector(images);
	item::free();
}
void animation::addImage(std::string pathToImage)
{
	images.push_back(IMG_Load(pathToImage.c_str()));
}
void animation::draw(SDL_Surface *dest)
{
	int index = step%images.size();
	if(images[index] != NULL)
	{
		SDL_BlitSurface(images[index], NULL, dest, &this->rect);
	}
}

void animation::draw(SDL_Surface * dest, int index, int x, int y)
{
	int rolledIndex = index % images.size();
	if(images[rolledIndex] != NULL)
	{
		SDL_Rect temp = this->rect;
		temp.x = x;
		temp.y = y;
		SDL_BlitSurface(this->images[rolledIndex], NULL, dest, &temp);
	}
	else
	{
		cout << "image not found while trying to draw.\n";
	}
}

void animation::next()
{
	limitter ++;
	if(!(limitter % skip))
	{
		step ++;
	}
}

void animation::remove(int index) // remove an image at index
{}
void animation::clear() // clear the entire animation
{
	images.clear();
}

bool animation::isEmpty() // check if animation is empty
{}
void animation::setFrameSkip(const int skipFrames)
{
	skip = skipFrames;
}



// ===============  txt ================
// A text rendering class


txt::txt()
{
	this->size = 14;
	this->rect.x = 10;
	this->rect.y = 10;
	this->color = {0,0,0};
	this->font = NULL;
}

void txt::free()
{
//	TTF_CloseFont(this->font);
	item::free();
}

txt::txt(std::string text, std::string fontPath, int x, int y)
{
	words = text;
	size = 14;
	this->color = {0,0,0};
	this->rect.x = x;
	this->rect.y = y;
	this->fontsFilePath = fontPath;
	this->font = TTF_OpenFont(fontPath.c_str(), size);
	this->image = TTF_RenderText_Solid(font, text.c_str(), color);
	this->safeTrackImage = this->image;
	TTF_CloseFont(this->font);
}

void txt::setText(std::string text)
{
	words = text;
	this->quickRender();
}


void txt::setFontSize(int size)
{
	this->size = size;
}

void txt::addFontSize(int change)
{
	this->size += change;
	if(this->size < 1)
	{
		this->size = 1;
	}
}

void txt::setFont(string fontPath)
{
	this->fontsFilePath = fontPath;
}

void txt::setColor(const SDL_Color textColor)
{
	this->color = textColor;
}

SDL_Color txt::getColor()
{
	return this->color;
}

string txt::getFont()
{
	return this->fontsFilePath;
}

TTF_Font* txt::getFontPtr()
{
	return this->font;
}

int txt::getFontSize()
{
	return this->size;
}

void txt::fullRender()
{
	TTF_CloseFont(this->font);
	if(this->image != NULL)
	{
		SDL_FreeSurface(this->image);
		this->image = NULL;
	}
	this->font = TTF_OpenFont(fontsFilePath.c_str(), size);
	if(!(words.empty()))
	{
		this->image = TTF_RenderText_Solid(font, words.c_str(), color);
	}
}

void txt::quickRender()
{
	if(this->image != NULL)
	{
		SDL_FreeSurface(this->image);
		this->image = NULL;
	}
	if(!(words.empty()))
	{
		this->image = TTF_RenderText_Solid(font, words.c_str(), color);
	}
}



void txt::addText(string text)
{
	this->words += text;
}


string * txt::getTextPtr()
{
	return &(this->words);
}

string txt::getText()
{
	return this->words;
}




//===================== TEXTBOX Class ==============



textBox::textBox()
{
	string temp = "";
	boundingBox.x = 10;
	boundingBox.y = 10;
	boundingBox.w = 200;
	boundingBox.h = 200;
	padding = 0;
	camera.x = 0;
	camera.y = 0;
	camera.w = boundingBox.w;
	camera.h = boundingBox.h;
	
	SDL_Surface * tmpImg = SDL_CreateRGBSurface(0, boundingBox.w, boundingBox.h, 32, 0,0,0,0);
	this->image = SDL_CreateRGBSurface(0, boundingBox.w, boundingBox.h, 32, 0,0,0,0);
	this->rect = boundingBox;
	background.setImage(tmpImg);
	background.setColor(70,0,0);
	shouldShowBkg = true;
	this->cursor = 0;
	this->rect.x = 10;
	this->rect.y = 10;

	currentLine = 0;
	
	SDL_Color col = {255, 200, 50, 0};
	lines.push_back(new txt());
	lines[0]->setFontSize(20);
	lines[0]->setFont("res/Acme/Acme-Regular.ttf");
	lines[0]->setColor(col);
	lines[0]->setPos(padding, padding);
	lines[0]->fullRender();
	lines[0]->setText(temp);

	this->cursorImg.setPos(padding, padding);
	this->cursorImg.setSize(3,lines[0]->getFontSize()); 
}

void textBox::showBkg(bool yesNo)
{
	shouldShowBkg = yesNo;
}

void textBox::handleEvent(SDL_Event & ev)
{
	if(SDL_GetModState() & KMOD_ALT && ev.type == SDL_KEYDOWN)
	{
		if(ev.key.keysym.sym == SDLK_UP)
		{
			scrollY(10);
		}
		else if(ev.key.keysym.sym == SDLK_DOWN)
		{
			scrollY(-10);
		}
		else if(ev.key.keysym.sym == SDLK_LEFT)
		{
			scrollX(10);
		}
		else if(ev.key.keysym.sym == SDLK_RIGHT)
		{
			scrollX(-10);
		}
	}
	else if(ev.type == SDL_KEYDOWN)
	{
		if(ev.key.keysym.sym == SDLK_BACKSPACE)
		{
			if(cursor > 0)
			{
				string * temp = lines[currentLine]->getTextPtr();
				string content = *temp;
				string letterDel = content.substr(cursor - 1, 1);
				int letterSize;
				TTF_SizeText(lines[currentLine]->getFontPtr(), letterDel.c_str(), &letterSize, NULL); 
				content = content.substr(0, cursor - 1);
				content += temp->substr(cursor, temp->length());
				lines[currentLine]->setText(content);
				cursor --;
				if(cursorImg.getPos().x < boundingBox.w/2 && camera.x < 0)
				{
					scrollX(letterSize);
				}
			}
			// concatenate strings, remove line from vector.
			else if(currentLine > 0)
			{
				cursor = lines[currentLine - 1]->getTextPtr()->length();
				lines[currentLine - 1]->addText(lines[currentLine]->getText());
				lines.erase(lines.begin() + currentLine);
				for(int i = currentLine; i < lines.size(); i ++)
				{
					lines[i]->move(0, lines[currentLine]->getFontSize() * -1);
				}
				currentLine --;
				TTF_SizeText(lines[currentLine]->getFontPtr(), lines[currentLine]->getText().c_str(), &camera.x, NULL); 
				if(camera.x > (boundingBox.w * 5)/6)
				{
					camera.x *= -1;
					camera.x += (boundingBox.w*5)/6;
				}
				else
				{
					camera.x = 0;
				}
			}
		}
		else if(ev.key.keysym.sym == SDLK_RETURN)
		{
			this->newLine();
			camera.x = 0;
		}
		else if(ev.key.keysym.sym == SDLK_LEFT)
		{
			if(cursor > 0)
			{
				cursor --;
			}
			else if(currentLine > 0)
			{
				currentLine --;
				cursor = lines[currentLine]->getTextPtr()->length();
			}
		}
		else if(ev.key.keysym.sym == SDLK_RIGHT)
		{
			if(cursor < lines[currentLine]->getTextPtr()->length())
			{
				cursor ++;
			}
			else if(currentLine + 1 < lines.size())
			{
				cursor = 0;
				currentLine ++;
				camera.x = 0;
				if(lines[currentLine]->getPos().y > (camera.h * 5) /6)
				{
					scrollY(10);
				}
			}
		}
		else if(ev.key.keysym.sym == SDLK_UP)
		{
			if(currentLine > 0)
			{
				currentLine --;
				int lineLen = (lines[currentLine]->getTextPtr())->length();
				if(cursor > lineLen)
				{
					cursor = lineLen;
				}
			}
		}
		else if(ev.key.keysym.sym == SDLK_DOWN)
		{
			if(currentLine < lines.size() - 1)
			{
				currentLine ++;
				int lineLen = (lines[currentLine]->getTextPtr())->length();
				if(cursor > lineLen)
				{
					cursor = lineLen;
				}
			}
		}
	}
	
	if(ev.type == SDL_TEXTINPUT)
	{
		lines[currentLine]->getTextPtr()->insert(cursor, ev.text.text);
		cursor ++;
		if(cursorImg.getPos().x > (boundingBox.w * 5)/6)
		{
			scrollX(-10);
		}
	}
	

}

void textBox::draw(SDL_Surface * dest)
{
	if(shouldShowBkg)
	{
		background.draw(this->image);
	}
	else
	{
		SDL_BlitSurface(dest, &this->rect, this->image, NULL);
	}
	// don't draw above the boundary or below. This saves no RAM,
	// but does cut down CPU use on larger files.
	int offset = 0;
	while(offset < lines.size())
	{
		SDL_Rect pos = lines[offset]->getPos();
		if(pos.y + pos.h < 0)
		{
			offset ++;
		}
		else
		{
			break;
		}
	}
	for(int i = offset; i < lines.size(); i ++)
	{
		
		lines[i]->quickRender();
		if(!(lines[i]->getTextPtr()->empty()))
		{
			SDL_Rect temp = lines[i]->getPos();
			temp.x += camera.x;
			temp.y += camera.y;
			SDL_BlitSurface(lines[i]->getImage(), NULL, this->image, &temp);
		}
		if(lines[i]->getPos().y + camera.y > boundingBox.h)
		{
			break;
		}
	}
// drawing the cursor image.	
	string sub = lines[currentLine]->getText().substr(0, cursor);
	int w;
	TTF_SizeText(lines[0]->getFontPtr(), sub.c_str(), &w, NULL);
	cursorImg.setPos(padding + w + camera.x, lines[currentLine]->getPos().y + camera.y);
	cursorImg.draw(this->image);

	SDL_BlitSurface(this->image, NULL, dest, &boundingBox);

}


void textBox::newLine()
{
		txt* tmp = new txt();
		string *prevTxt = lines[currentLine]->getTextPtr();
		string nextTxt;
		nextTxt = prevTxt->substr(cursor, prevTxt->length());
		prevTxt->erase(cursor, prevTxt->length());


		int fontSizeY = lines[0]->getFontSize();
		tmp->setFont(lines[0]->getFont());
		tmp->setColor(lines[0]->getColor());
		tmp->setFontSize(fontSizeY);
		tmp->fullRender();
		for(int i = currentLine + 1; i < lines.size(); i ++)
		{
			lines[i]->move(0, fontSizeY);
		}
		tmp->setPos(padding, padding + lines[currentLine]->getPos().y + tmp->getFontSize());
		currentLine ++;
		lines.insert(lines.begin() + currentLine, tmp);
		tmp->setText(nextTxt);
		cursor = 0;
	
}

void textBox::scrollX(int pixLeng)
{
	if(camera.x + pixLeng <= 0)
	{
		camera.x += pixLeng;
	}
}

void textBox::scrollY(int pixLeng)
{
	if(camera.y + pixLeng <= 0)
	{
		camera.y += pixLeng;
	}
}


void textBox::setBoundary(int x, int y, int w, int h)
{
	boundingBox.x = x;
	boundingBox.y = y;
	boundingBox.w = w;
	boundingBox.h = h;
	this->rect = boundingBox;
	this->camera.x = 0;
	this->camera.y = 0;
	this->camera.w = boundingBox.w;
	this->camera.h = boundingBox.h;
	this->background.setSize(w, h);

	SDL_FreeSurface(this->image);
	this->image = SDL_CreateRGBSurface(0,boundingBox.w, boundingBox.h, 32, 0,0,0,0);
}

void textBox::setPadding(int between)
{
	lines[0]->move(between - padding, 0);
	padding = between;
}

void textBox::changeFontSize(int change)
{
	for(int i = 0; i < lines.size(); i ++)
	{
		lines[i]->addFontSize(1);
		lines[i]->move(0, i);
		lines[i]->fullRender();
	}
}

void textBox::setBoundary(SDL_Rect bounds)
{
	boundingBox.x = bounds.x;
	boundingBox.y = bounds.y;
	boundingBox.w = bounds.w;
	boundingBox.h = bounds.h;
}

void textBox::setBkgColor(int r, int g, int b)
{
	background.setColor(r, g, b);
}



//====================== SPEED CLASS ================


speed::speed()
{
	this->fps = 30;
	this->ticks = SDL_GetTicks();
	this->fc = 0;
	this->redux = 0;
}

void speed::updateTick()
{
	ticks = SDL_GetTicks();
}

void speed::printFPS(SDL_Surface * dest, const string fontPath, int x, int y)
{
	stringstream ss;
	ss.str("");
	int time = SDL_GetTicks();
	ss << "Speed: " << ((double)this->fc) / (((double) time - this->redux)/ 1000.0) << "FPS";
	txt temp(ss.str(), fontPath, x, y);
	temp.stamp(dest, x, y);
	temp.free();
}

void speed::printGT(SDL_Surface * dest, const string fontPath, int x, int y)
{
	stringstream ss;
	ss.str("");
	ss << "Game-Time: ";
	unsigned int gt = SDL_GetTicks()/1000;
	if( gt < 60)
	{
		ss << gt << "s";
	}
	else if(gt < 3600)
	{
		ss << gt/60 << "m:" << gt % 60 << "s";
	}
	else
	{
		ss << gt/3600 << "h:" << (gt % 60)/60 << "m:" << gt % 36000 << "s";

	}

	txt temp(ss.str(), fontPath, x, y);
	temp.stamp(dest, x, y);
	temp.free();

}

void speed::limitFPS()
{
	int current = SDL_GetTicks();
        if((1000/this->fps) > current - this->ticks)
        {
                SDL_Delay(1000/this->fps - (current - this->ticks));
        }
	this->ticks = SDL_GetTicks();
}

 
//======================= DICE CLASS ================

dice::dice()
{
	srand(time(NULL));
	scaled = false;
	sideUp = 1;

}

dice::dice(int sides)
{
	srand(time(NULL));
	maxSide = sides;
	scaled = false;
	sideUp = 1;
	for(int i = 0; i < sides; i ++)
	{
		sideImages.push_back(NULL);
	}
}

void dice::free()
{
	freeVector(sideImages);
	item::free();
}

int dice::roll(const bool mustChange)
{
	// For the animation of a slow roll, the sides must change
	// otherwise actual randomosity is desired.
	if(mustChange) 
	{
		int oldside = sideUp;
		while(oldside == sideUp)
		{
			sideUp = rand() % maxSide;
		}
	}
	else
	{
		sideUp = rand() % maxSide;
	}
	return sideUp;
}

int dice::roll()
{
	sideUp = rand() % maxSide;
	return sideUp;
}

void dice::setImage(int side, string path)
{
	SDL_Surface * temp = IMG_Load(path.c_str());
	sideImages[side] = temp;

}

int dice::getLastRoll()
{
	return sideUp;
}

void dice::draw(SDL_Surface* dest)
{
	if(sideImages[sideUp] != NULL)
	{
		this->image = sideImages[sideUp];
		item::draw(dest);
	}
	else
	{
		cout << "Dice side " << sideUp << " Has no image\n";
	}
}

bool dice::playRoll(int &frames, int speed)
{
	if(frames > 0 )
	{
		if(!(frames % speed))
		{
			roll(true);
		}
		frames --;
		return true;
	}
	return false;
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

void button::free()
{
	freeVector(stateImg);
	item::free();
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

int xyToSingle(int x, int y, int gridWidth)
{
	if(x > 0 && y > 0 && gridWidth > 1)
	{
		return y * gridWidth + x;
	}
}

bool writeImage(SDL_Surface * src, char * fileName)
{
	FILE *fp = fopen(fileName, "wb");
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);
	png_text desc;
	desc.compression = PNG_TEXT_COMPRESSION_NONE;
	desc.key = fileName;
	desc.text = fileName;
	png_set_text(png, info, &desc, 1);
	png_bytep byte = (png_bytep) malloc(3 * src->clip_rect.w * sizeof(png_byte));
	setjmp(png_jmpbuf(png));
	png_init_io(png, fp);
	png_set_IHDR(png, info, src->clip_rect.w, src->clip_rect.h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png, info);
	for(int y = 0; y < src->clip_rect.h; y ++)
	{
		for(int x = 0; x < src->clip_rect.w; x++)
		{
			unsigned char r, g, b;
			SDL_GetRGB(((unsigned int*) src->pixels)[xyToSingle(x, y, src->clip_rect.w)], src->format, &r, &g, &b);
			byte[x*3] = r;
			byte[x*3+1] = g;
			byte[x*3+2] = b;
		}
		png_write_row(png, byte);
	}
	png_free_data(png, info, PNG_FREE_ALL, -1);
	png_destroy_write_struct(&png, (png_infopp) NULL);
	free(byte);
	return 0;
}


double xFromPolar(double length, double angle)
{
	return length * cos(angle);
}

double yFromPolar(double length, double angle)
{
	return length * sin(angle);
}

void drawCircle(SDL_Surface * dest, unsigned int color, double x, double y, double radius)
{
	int ox, oy;
	for(double i = 0; i < 360; i += 90/radius)
	{
		ox = x + xFromPolar(radius,  i);
		oy = y + yFromPolar(radius,  i);
		putPixel(dest, ox, oy, color);
	}
}


bool isInside(int x, int y, SDL_Surface * surf)
{
	if((x >= 0 && x < surf->w) && (y >= 0 && y < surf->h))
	{
		return true;
	}
	return false;
}



void putPixel(SDL_Surface* dest, int x, int y, unsigned int color)
{
	if(isInside(x, y, dest))
	{
		((unsigned int *)dest->pixels)[y * dest->pitch / 4 + x] = color;
	}
}


void drawLineLow(SDL_Surface * dest, unsigned int color, int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if(dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	int D = 2*dy - dx;
	int y = y0;
	for(int x = x0; x < x1; x ++)
	{
		putPixel(dest, x, y, color);
		if(D > 0)
		{
			y = y + yi;
			D = D - 2*dx;
		}
		D = D + 2*dy;
	}
}
void drawLineHigh(SDL_Surface * dest, unsigned int color, int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if(dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = 2*dx - dy;
	int x = x0;

	for(int y = y0; y < y1; y ++)
	{
		putPixel(dest, x, y, color);
		if(D > 0)
		{
			x = x + xi;
			D = D - 2*dy;
		}
		D = D + 2*dx;
	}
	

}

void drawLine(SDL_Surface * dest, unsigned int color, int x0, int y0, int x1, int y1)
{
	if(abs(y1-y0) < abs(x1-x0))
	{
		if(x0 > x1)
		{
			drawLineLow(dest,color, x1, y1, x0, y0);
		}
		else
		{
			drawLineLow(dest, color, x0, y0, x1, y1);
		}
	}
	else
	{
		if(y0 > y1)
		{
			drawLineHigh(dest,color, x1, y1, x0, y0);
		}
		else
		{
			drawLineHigh(dest, color, x0, y0, x1, y1);
		}
	}

}


void drawBox(SDL_Surface * dest, SDL_Rect rect, unsigned int color)
{
	drawLine(dest, color, rect.x, rect.y, rect.x, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y, rect.x + rect.w, rect.y);
	drawLine(dest, color, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
}

void drawBox(SDL_Surface * dest, SDL_Rect rect, int r, int g, int b)
{
	unsigned int color = SDL_MapRGB(dest->format, r, g, b);
	drawLine(dest, color, rect.x, rect.y, rect.x, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y, rect.x + rect.w, rect.y);
	drawLine(dest, color, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
	drawLine(dest, color, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
}




