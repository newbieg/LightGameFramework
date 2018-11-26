/* Some basic game classes built upon SDL2.
 * Should work as a time-saver and game organization tool
 * There are two parent classes: 
 * 	item - basically any item of interest whether it be a drawable thing
 * 		or a source of sound, text, or other game asset.
 * 	group - a list of items, these should generally have some basic
 * 		similarity to eachother, such as they are drawn at the same time,
 * 		or share similar functions that need to be called together.
 * 		Perhaps that each needs to be checked for a mouseclick...
 * 		
 * 
 */

// IMPORTANT REMINDER:
// You the user must init the SDL libraries once in your program,
// This includes SDL_TTF AND SDL_IMAGE

#ifndef LITEGAMEFRAMEWORK_0_0_1
#define LITEGAMEFRAMEWORK_0_0_1

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>

enum ButtonStates
{
	BTN_UP,
	BTN_HOVER,
	BTN_DOWN,
	BTN_CLICK, 
	BTN_DBCLICK
};

// dest = surface that we are optimizing returned surface for. 
// In most cases this is going to be a pointer to the screen's surface.
// Use optLoad if image is going to be stretched using SDL_BlitScaled(etc.);
SDL_Surface * optLoad(std::string path, const SDL_Surface* dest );



// startTime = SDL_GetTicks();
// The above line should be used at the very beginning of whatever loop you
// wish to place a fram-rate limit on.
// fps is the desired frames per second for the loop to run at

void speedLimit(const int fps, const int startTime);




// controls for a drawable item or sprite
class item
{
	public:
	item();
	virtual ~item();
	item(unsigned int color, int x, int y, int w = 100, int h = 100);
	item(int x, int y, int w = 100, int h = 100); 
	virtual void update();
	virtual void draw(SDL_Surface* dest);
	bool operator==(const item &other) const; // it's early in the moring, I'll implement later.
	void setSize(int w, int y);
	void setPos(int x, int y);
	void getPos(int & x, int & y);
	SDL_Rect getPos();
	void getSize(int & w, int & h);
	SDL_Rect getSize();
	SDL_Surface* getImage() const;

	// Beware, setImage changes the value that the == operator currently relies on.
	void setImage(SDL_Surface* freshImage); // the image itself should be loaded beforehand. 
	bool checkCollision(item & other);
	bool isInside(item & other);
	bool isInside(int x, int y);
	int getCenterX();
	int getCenterY();
	unsigned int setColor(int r, int g, int b);

	void move(int x, int y);

	virtual void load(std::ifstream & file); // returns a copy of itself
	virtual void save(std::ofstream & file);


	protected:
	SDL_Surface * image;
	SDL_Rect rect;

};


// Not implemented
class animation : public item
{
	public:
		void addImage(std::string pathToImage);
		void draw(); // Conditionally draw next frame if time is right.
		void remove(int index); // remove an image at index
		void clear(); // clear the entire animation
		bool isEmpty(); // check if animation is empty
		void setFrameRate(int frameRateSet); // change animation speed

	protected:
		std::vector <SDL_Surface*> images;
		int frameRate;
		int lastFrame;

};


// could use some improvement, 
// This is how I imagine a button might pass a click to a function... 
class button: public item
{
	public:
		button();

		bool eventCheck(SDL_Event * e); // return true if event is interacting with button
		virtual int getState(); // return a BUTTON_STATE enumerative
		virtual void onClick(void (*function)());
		virtual void onDblClick(void (*function)());

		virtual void setImage(int BTN_ENUM_FLAG, SDL_Surface *theImage);

	private:
		int BTN_State;
		std::vector <SDL_Surface*> stateImg;
		void (*activated)();
		void (*dblActivated)();
		bool connected;
		bool dblConnected;
};




class txt: public item
{
	public:
		txt();
		txt(std::string text, int x, int y);
		void setTxt(std::string text);
		void setFontSize(int size);
		void setColors(unsigned int foreground, unsigned int background); 
		void setFont(std::string path);

		// void slowDraw(SDL_Surface* dest , int fpl); // instead of instantly printing, type each letter on a frame per second scale (fpl == frames per letter) ... have decided that this should wait until animation class is set up...



	private:
		std::string words;
		int size;
		SDL_Color color;
		TTF_Font *font;


};

class dice: public item
{
	public:
	dice();
	dice(int sides);
	int roll();
	int getLastRoll();
	virtual int playRoll(); // play an animation of the dice rolling
	private:
	int sideUp;
	animation rolling;


};


// controls for a small or large group of items, 
// such as those available on a given board/level
class group
{
	public:
	group copy(); // return a copy of this group

	void add(item *addItem);
	void remove(item removItem);
	int size(); // return number of items in group
	virtual void update(); // update all 
	virtual void draw(SDL_Surface * dest); // draw all
	void clear(); // remove all items

	bool isEmpty(); // check if group is empty
	bool has(item hasIt); // check if hasIt is in the group
	void move(int x, int y); // move the x,y by an increment of given
	group getColision(item& colider);
	group getClicked(int x, int y);
	item* getItem(int index);
	std::vector <item*> getItems();

	virtual void load(std::string fileName);
	virtual void save(std::string fileName);
	

	protected:
	std::vector <item*> items;



};






#endif
