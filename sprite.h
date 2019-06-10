/* Some basic game classes built upon SDL2.
 * Should work as a time-saver and game organization tool
 * There are two parent classes: 
 * 	item - basically any item of interest whether it be a drawable thing
 * 		or a source of sound, text, or other game asset.
 * 	group - a list of items, these should generally have some basic
 * 		similarity to eachother, such as they are drawn at the same time,
 * 		or share similar functions that need to be called together.
 * 		Perhaps that each needs to be checked for a mouseclick...
 n* 		
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
#include "SDL2/SDL_mixer.h"
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>

const int TILESIZE = 32;

enum WindowTransforms
{
	WT_BLOCKS, // replace old with squares containing new
	WT_BLOCKSFALLING, // new screen replaced by falling blocks
	WT_FADE, // slow fade by averaging the two screens
	WT_FRACTURE, // Might also be called window blinds
	WT_FRACTURESLIDE, // slide the blinds, see previous
	WT_OPENDOOR, // opens from mid-screen
	WT_SLIDEDOWN, // slides old screen in given direction
	WT_SLIDELEFT,
	WT_SLIDERIGHT,
	WT_SLIDEUP,
	WT_SNOW // small random colors that eventually match new scr
};

enum ButtonStates
{
	BTN_UP,
	BTN_HOVER,
	BTN_DOWN,
	BTN_CLICK, 
	BTN_DBCLICK
};

enum ChrStates
{
	CHR_WALK,
	CHR_RUN,
	CHR_FLY,
	CHR_JUMP,
	CHR_CLIMB,
	CHR_SWIM,
	CHR_ATTACK,
	CHR_PUSH,
	CHR_KICK,
	CHR_MAGIC,
	CHR_SHOOT,
	CHR_BORED,
	CHR_MAX = CHR_BORED * 5,
	// the following are direction/combinational.
	// use as a flag and one of the above enums
	// CHR_LEFT | CHR_WALK == character WALKing LEFT
	CHR_BELOW = 1 << 8, 
	CHR_UP = 1 << 9,
	CHR_DOWN = 1 << 10, 
	CHR_LEFT = 1 << 11, 
	CHR_RIGHT = 1 << 12,

};

enum tileStates
{
	TILE_SOLID,
	TILE_NEUTRAL,
	TILE_WATER,
	TILE_CODE
};


// dest = surface that we are optimizing returned surface for. 
// In most cases this is going to be a pointer to the screen's surface.
// Use optLoad if image is going to be stretched using SDL_BlitScaled(etc.);
SDL_Surface * optLoad(std::string path, const SDL_Surface* dest );

// initializes the SDL libraries needed for the FrameWork to, well, work
void initFramework();

// Close the SDL libraries at the end of the program, closeFramework()
void closeFramework();

// convert a cartesian point down to a single dimensional array
int xyToSingle(int x, int y);

// Write an SDL_Surface to a png file. No other fileType
// is supported at this time. Use for a single snapshot of an item 
// or of the screen. (Not a fast function, use sparingly) 
bool writeImage(SDL_Surface * src, char* fileName);

// startTime = SDL_GetTicks();
// The above line should be used at the very beginning of whatever loop you
// wish to place a fram-rate limit on.
// fps is the desired frames per second for the loop to run at

void speedLimit(const int fps, int &startTime);

// test that a point is inside a surface drawing area
bool isInside(int x, int y, SDL_Surface* surf);
void swap(int & x, int & y);
void donoth();

// draw a pixel of color (which should already be in the correct format) onto dest at (x, y)
void putPixel(SDL_Surface* dest, int x, int y, unsigned int color);

// draw a line on dest from (x1, y1) to x2, y2)
void drawLine(SDL_Surface *dest, unsigned int color, int x1, int y1, int x2, int y2);
void scanLineDraw(SDL_Surface * dest, unsigned int color, int x, int y, int x2);

void drawBox(SDL_Surface * dest, SDL_Rect rect, unsigned int color);
void drawBox(SDL_Surface * dest, SDL_Rect rect, int r, int g, int b);

// use when something went wrong and you want to exit program.
void oshit();


// Polar Vs. Cartesian Functions
// Use these functions to convert between polar and cartesian points.
double xFromPolar(double length, double angle);
double yFromPolar(double length, double angle);
double rFromCartesian(double x, double y);
double phiFromCartesion(double x, double y);

void drawCircle(SDL_Surface * dest, unsigned int color, double x, double y, double radius);

// controls for a drawable item or sprite
// needsUpdate true:  image/position/etc is modified and needs window attention.
class item
{
	public:
	item(const item &copy);
	item(SDL_Rect size);
	item();
	virtual ~item();
	item(unsigned int color, int x, int y, int w = 100, int h = 100);
	item(int x, int y, int w = 100, int h = 100); 

	// should return a rect that requires screen update
	virtual SDL_Rect update();
	virtual void draw(SDL_Surface* dest);
	// stamp image onto dest surface at (x, y)
	virtual void stamp(SDL_Surface * dest, int x, int y);
	// load sub-image onto another item's surface (spriteSheet)
	virtual void stamp(SDL_Surface * dest, SDL_Rect sub, int x, int y);
	bool operator==(const item &other) const;
	void setSize(int w, int y);
	void setPos(int x, int y);
	void getPos(int & x, int & y);
	void setRect(SDL_Rect r);
	SDL_Rect getPos() const;
	void getImgSize(int & w, int & h);
	void getSize(int & w, int & h);
	SDL_Rect getSize();
	SDL_Surface* getImage() const;
	virtual void free();
	//returns an image composed of the clipped area from source image 
	// (use for Sprite sheets)
	SDL_Surface* getClip(SDL_Surface* source, SDL_Rect clipRect);

	virtual void setImage(SDL_Surface* freshImage); 
	virtual void setImage(std::string path); 

	virtual void addColor(const unsigned int r, const unsigned  int g, const unsigned int b); 
	// Set maxOut to true if you don't want the color to circle to zero on
	// a larg addition
	virtual void addColor(bool maxOut, const unsigned int r, const unsigned  int g, const unsigned int b); 
	// given a mask image, only 
//	virtual void xorImage(SDL_Surface * other); 
	// combine other image with current one. 
	virtual void averageImage(SDL_Surface * other);
	virtual bool checkCollision(const item & other) const;
	virtual bool checkCollision(const SDL_Rect & boarder) const;
	virtual bool isInside(item & other);
	virtual bool isInside(int x, int y);
	int getCenterX();
	int getCenterY();
	unsigned int setColor(int r, int g, int b);
	unsigned int setColor(int r, int g, int b, int a);
	unsigned int setColor(unsigned int mappedColor);

	void move(int x, int y);
	
	virtual void load(std::ifstream & file);
	virtual void save(std::ofstream & file);

	bool needsUpdate; 
	void setDrag(bool beingDragged);
	bool getDrag();
	std::string name;
	bool scaled;

	/*
	// For the recorder
	// All calls to a recording should be made from a single thread
	virtual void saveRecording(std::ofstream &file);
	virtual void loadRecording(std::ifstream &file);
	virtual void playRecording();
	// use to start/stop recording
	virtual void record(bool state);
	// An auto-save version of record, pushes to file
	// and clears the previous content, better for long recordings
	virtual void recordAutoSave(std::ofstream &file, int everyTFrames);
	bool recordKey(int time);
	*/
	unsigned long getID() const;
	static unsigned long getItemCount();


	protected:
	SDL_Surface * image;
	SDL_Surface * safeTrackImage;
	SDL_Rect rect;
	unsigned int color;

	bool dragging; // use as in-game drag/drop flag

	/*
	// For the recorder
	bool recordingState;
	int recTFrames;
	std::vector<int> recX;
	std::vector<int> recY;
	std::vector<int> recKey; // timing from start that action happens.
	*/

	static unsigned long itemCount;
	unsigned long id;
};


class window : public item
{
	public:
		window();
		window(std::string title, int width, int height);
		window(std::string title, int width, int height, unsigned int flags);

		SDL_Surface * getImage();
		void linkScreen(SDL_Surface ** screen);

		// Update the window's contents with the screen
		// return the size of the screen
		SDL_Rect update();
		// Update the window's contents with the screen
		void draw(); 
		void setTitle(std::string title);
		std::string getTitle();
		double getOpacity();
		// hides window, simulates window closing.
		// nice way to store a window for later use.
		void hide();

		// window is hidden by default
		void show();

		// actually closes window and destroys it.
		void close();

		// checks for window-type events and handles actions
		// relative to them.
		// Return: true if event pertains to this window.
		bool handleEvent(SDL_Event &ev);

		//considering implementing as a linked list so that 
		//all windows could be closed by calling the
		//following: void closeAll();

//		void move(int x, int y);


//		toggleFS() - toggle full screen mode;
		void addOpacity(double change);
		void toggleFS();
	private:
		double opacity;
		unsigned int windowFSFlag;
		unsigned int windowResizeFlag;
		std::string windowName;
		SDL_Window* wind;
		SDL_Surface** linkedScr;
		bool hidden;
		bool closeAll;
};



// Not implemented
class animation : public item
{
	public:
		animation();
//	virtual ~animation();
		void addImage(std::string pathToImage);
		void addImage(std::string path, SDL_Rect camera);
		void draw(SDL_Surface * dest);
		void draw(SDL_Surface * dest, int index, int x, int y);
		void next();
		void remove(int index); // remove an image at index
		// free the resource images
		virtual void free();
		void clear(); // clear the entire animation
		bool isEmpty(); // check if animation is empty
		void setFrameSkip(const int ignoreFrames); // change animation speed

	protected:
		std::vector <SDL_Surface*> images;
		int step;
		int limitter;
		int skip;

};


class sound: public item
{
	public:

	private:

};

// could use some improvement, 
// This is how I imagine a button might pass a click to a function... 
class button: public item
{
	public:
		button();
//	virtual ~button();
		bool eventCheck(SDL_Event * e); // return true if event is interacting with button
		virtual int getState(); // return a BUTTON_STATE enumerative
		virtual void onHover(void (*function)());
		virtual void onClick(void (*function)());
		virtual void onDblClick(void (*function)());

		virtual void setImage(int BTN_ENUM_FLAG, SDL_Surface *theImage);
		virtual void setImage(int BTN_ENUM_FLAG, std::string imagePath);
		virtual void free();

	private:
		int BTN_State;
		std::vector <SDL_Surface*> stateImg;
		void (*activated)();
		void (*dblActivated)();
		void (*hoverActivated)();
		bool connected;
		bool dblConnected;
		bool hoverConnected;
};



class txt: public item
{
	public:
		txt();
//		virtual ~txt();
		txt(std::string text, std::string fontPath, int x, int y);
		// call when a change of font is required
		void fullRender();
		// call when a simple change in text is made
		void quickRender();
		void setText(std::string text);
		void setFontSize(int size);
		void addFontSize(int change);
		void setFont(std::string fontPath);
		std::string getFont();
		TTF_Font * getFontPtr();
		int getFontSize();
		void setColor(const SDL_Color textColor); 
		SDL_Color getColor();
		virtual void free();


		// Add text to end of words;
		void addText(std::string text); 
		// insert Text directly after pos given
		void insertText(std::string, int pos);
		// return a text pointer so another item can manipulate
		// the contents directly. Other item must call reset when
		// done. 
		std::string * getTextPtr();
		// Return a copy of the text being used.
		std::string getText();

		// void slowDraw(SDL_Surface* dest , int fpl); // instead of instantly printing, type each letter on a frame per second scale (fpl == frames per letter) ... have decided that this should wait until animation class is set up...
	private:
		std::string words;
		int size;
		SDL_Color color;
		TTF_Font *font;
		std::string fontsFilePath;
};


// An item with a solid background which allows for
// text entry from the user.
class textBox : public item
{
	public:
	explicit textBox(int x, int y, int w, int h) : item(x, y, w, h) {};
	textBox();
	void showBkg(bool yesNo);
	// If this item has keyboard focus, the envent will 
	// return is a rect that indicates where a redraw is required
	void handleEvent(SDL_Event &ev);
	void draw(SDL_Surface * dest);
	// insert a new line where the cursor sits.
	void newLine();
	// pixLeng is the length in pixels to scroll left. - negative = right.
	void scrollX(int pixLeng); 
	void scrollY(int pixLeng); 
	// change the font size of all text. Var change is relative to current
	// size and can be + or -
	void changeFontSize(int change);
	void setBoundary(int x, int y, int w, int h);
	void setBoundary(SDL_Rect Bounds);
	// the padding between each line
	void setPadding(int between);
	void setBkgColor(int r, int g, int b);

	private:
	std::vector <txt*> lines;
	item background;
	bool shouldShowBkg;
	unsigned int cursor; // position in content that is being written to
	item cursorImg; // The cursor image shows where next char is typed.
	unsigned int bkgColor;
	int padding;
	SDL_Color fgColor;
	int currentLine;
	// keep visible text inside this invisible box
	SDL_Rect boundingBox;
	// we're redoing a bit of what was done with board class,
	// camera is how we will control scrolling.
	SDL_Rect camera;
};



// I might need to wait on this until I come up with 
// a modified txt which allows for each letter to be a different color...
// WORK IN PROGRESS
class marquis : item
{
	public:
		marquis();
		void addSpeed(int change);
		void changeDirection();
		void setSpeed(int newSpeed);
		txt writing;
		void draw(SDL_Surface * dest);

	private:
		int speed;
		SDL_Rect camera;
		// should text blink on/off 
		bool blink;
		// should the entire text run through to empty before loop?
		bool clearRunBeforeNext; 
};



// Dialogue box. You. Dirty mind.
// Not implemented
class dBox : public item
{
	public:
	void setText(std::string text);
	void slowType(SDL_Surface * dest, std::string text, int skipFrames);
	void step();
	void print(SDL_Surface * dest, int x, int y);

	private:
	std::vector <std::string> message;
};


class speed
{
	public:
	speed();
	int fc; // frameCount
	int internalFC;
	int fps; // frames per second limit
	int ticks; // time since game start(update each loop)
	int redux; // use instead for calculating from start of loop...
	void updateTick();
	// fontPath must path to a ttf file. For resources, check out
	// google fonts, many have an open-source license 
	void printFPS(SDL_Surface * dest, const std::string fontPath, int x, int y);
	void printGT(SDL_Surface * dest, const std::string fontPath, int x, int y);
	void limitFPS();

};

class dice: public item
{
	public:
	dice();
	dice(int sides);
//	virtual ~dice();
	// mustChange true = don't repeat sides from one roll to next
	// better for slow animation of a roll.
	int roll(const bool mustChange);
	int roll();
	void setImage(int sideNumber, std::string path);
	int getLastRoll();
	virtual bool playRoll(int & framesLeft, int speed); // play an animation of the dice rolling
	void draw(SDL_Surface * dest);
	virtual void free();

	private:
	int sideUp;
	int maxSide;
	int frameCount;
	std::vector <SDL_Surface *> sideImages;


};


// controls for a small or large group of items, 
// such as those available on a given board/level
class group
{
	public:
	group copy(); // return a copy of this group

	void add(item *addItem);
	void add(group concat);
	void remove(item removItem);
	int size(); // return number of items in group
	virtual void update(); // update all 
	virtual void draw(SDL_Surface * dest); // draw all
	// free all images in the given group.
	// Use when all resources in this group are no longer needed.
	// calls free on all items in the group.
	// all child classes should implement a clear function.
	// which calls it's parent's free as well
	virtual void free();
	// removes all items from the group.
	// if the resources in the group are no longer needed
	// (eg. you are using the group as a resource manager)
	// then call free first, then clear.
	void clear(); 
	bool isEmpty(); // check if group is empty
	bool has(item hasIt); // check if hasIt is in the group
	void move(int x, int y); // move the x,y by an increment of given
	group getCollision(item& colider);
	// type-safe, but will return all objects under 
	// 	point (x, y), not just those that accept mouseclicks.
	// Group that calls this should only contain objects which 
	// a click is expected to take place.
	group getClicked(int x, int y);
	group getDrags();
	item* getItem(int index);
	// returns position in group, otherwise a negative number 
	// if not present.
	int getIterator(item &lookFor);
	std::vector <item*> getItems();
	virtual void load(std::string fileName);
	virtual void save(std::string fileName);
	// return a list of rectangles requiring update on window.
	// User must pass to SDL_UpdateWindowRects();
	std::vector <SDL_Rect> getUpdateRegion();
	std::vector <SDL_Rect> getRects();
	void clearUpdateRegion();
	

	protected:
	std::vector <item*> items;
	std::vector <SDL_Rect> updateRects; 



};


// A tile is basically a set-size image that a character
// is able to interact with. Most basic interactions include
// either can walk on top, or colide against.
// NPC's could inherit from tile class
// if further interaction must be done, overload the interact funtion.
// each tile is going to be stackable 

class tile : public item
{
	public:
	explicit tile(unsigned int color, int x, int y, int w, int h) : item(color, x, y, w, h) {};
	explicit tile(int x, int y, int w, int h) : item(x, y, w, h) {};
	virtual void free();
	void setState();
	void swapLayers(int first, int second);
	void add(item layer);
	virtual void draw(SDL_Surface * dest);
	// use to draw an item onto another surface (dest) at (x, y)
	virtual void stamp(SDL_Surface * dest, int x, int y);
	virtual void drawLayer(SDL_Surface * dest, int depth);

	private:
	static int TileWidth, TileHeight; // default is 32X32
	std::vector <item> layers;
	void (*within)(); // code to run when user fully within
	void (*contact)(); // code to run when user makes contact at the boarders
	void (*mouseClick)(); // code to run when... Oh, you get the idea...
	void (*event)(SDL_Event);

};



// maintians a grid of tiles and items
// some of these items can be moved
// there's also the option of using a background image as the board
class board : public item
{
	public:
	board();
	// w and h are tile positions, tileSizeW is the width in pixels.
	explicit board(int x, int y, int w, int h) : item(x, y, w, h) {};
	// x and y are the top left pad, w and h are the pad to the bottom right
	void setPadding(int x, int y, int w, int h);
	// Must set Dimensions, w and h are 32 (TILESIZE) pixels per increment
	void setDimensions(int w, int h);
	void setCamera(SDL_Rect windowSize);
	void setCamera(int x, int y, int w, int h);
	SDL_Rect* getCamera();
	virtual void draw(SDL_Surface * dest);
	virtual void drawDriven(SDL_Surface* dest, int frameCount);
	// a tile that is going to be used profusely and therefore can just
	// fill the whole board.
	void fillBoard(item toUse);
	// stamp tile direct on board image
	void addTile(item toAdd, int x, int y);
	// stamp tile direct on board, with finer control of where in the tile
	// subx/suby are relative to the top left of the tile specified with 
	// x and y.
	void addTile(item toAdd, int x, int y, int subx, int suby);
	void addDriven(item *toAdd, int x, int y); // draw tile at pos relative to a tile, use for animated items
	// add new layers to the main drawable
	void addLayers(int count);
	void move(int x, int y);
	// Animation driven tiles/items, these should be drawn each frame.
	// to keep fps up, this group should be kept smaller.
	group driven;
	// may be redundant as they can be done with  a combination of
	// these are picked up when activated.
	group items;
	// Put characters in the walk group, they will apear to move
	// oposite to items and tiles
	group walk;
	// static tiles that compose the main image for the board.
	// this group should be drawn once before the game loop, then
	// the camera will be used to move the whole image.
	std::vector <group> layer;
	// Not drawn, these are tiles that need to be checked for colision.
	group solids;

	private:
	SDL_Rect camera;
	int tw, th;
	SDL_Rect padding;
};

#endif
