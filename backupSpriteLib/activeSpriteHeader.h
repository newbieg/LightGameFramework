Reading file sprite.h
228
#ifndef LITEGAMEFRAMEWORK_0_0_1
#define LITEGAMEFRAMEWORK_0_0_1
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
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
SDL_Surface * optLoad(std::string path, const SDL_Surface* dest );
int xyToSingle(int x, int y);
void speedLimit(const int fps, int &startTime);
void oshit();
class item
{
	public:
	item();
	virtual ~item();
	item(unsigned int color, int x, int y, int w = 100, int h = 100);
	item(int x, int y, int w = 100, int h = 100); 
	virtual SDL_Rect update();
	virtual void draw(SDL_Surface* dest);
	bool operator==(const item &other) const;
	void setSize(int w, int y);
	void setPos(int x, int y);
	void getPos(int & x, int & y);
	SDL_Rect getPos();
	void getImgSize(int & w, int & h);
	void getSize(int & w, int & h);
	SDL_Rect getSize();
	SDL_Surface* getImage() const;
	void setImage(SDL_Surface* freshImage); 
	bool checkCollision(item & other);
	bool isInside(item & other);
	bool isInside(int x, int y);
	int getCenterX();
	int getCenterY();
	unsigned int setColor(int r, int g, int b);
	void move(int x, int y);
	virtual void load(std::ifstream & file);
	virtual void save(std::ofstream & file);
	bool needsUpdate; 
	void setDrag(bool beingDragged);
	bool getDrag();
	std::string name;
	bool scaled;
	virtual void saveRecording(std::ofstream &file);
	virtual void loadRecording(std::ifstream &file);
	virtual void playRecording();
	virtual void record(bool state);
	virtual void recordAutoSave(std::ofstream &file, int everyTFrames);
	bool recordKey(int time);
	unsigned long getID() const;
	static unsigned long getItemCount();
	protected:
	SDL_Surface * image;
	SDL_Rect rect;
	bool dragging; // use as in-game drag/drop flag
	bool recordingState;
	int recTFrames;
	std::vector<int> recX;
	std::vector<int> recY;
	std::vector<int> recKey; // timing from start that action happens.
	static unsigned long itemCount;
	unsigned long id;
};
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
class button: public item
{
	public:
		button();
		bool eventCheck(SDL_Event * e); // return true if event is interacting with button
		virtual int getState(); // return a BUTTON_STATE enumerative
		virtual void onHover(void (*function)());
		virtual void onClick(void (*function)());
		virtual void onDblClick(void (*function)());
		virtual void setImage(int BTN_ENUM_FLAG, SDL_Surface *theImage);
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
class clickable : public item
{
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
	int roll(const bool mustChange);
	int roll();
	void setImage(int sideNumber, std::string path);
	int getLastRoll();
	virtual bool playRoll(int & framesLeft, int speed); // play an animation of the dice rolling
	void draw(SDL_Surface * dest);
	private:
	int sideUp;
	int maxSide;
	int frameCount;
	std::vector <SDL_Surface *> sideImages;
};
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
	void clear(); // remove all items
	bool isEmpty(); // check if group is empty
	bool has(item hasIt); // check if hasIt is in the group
	void move(int x, int y); // move the x,y by an increment of given
	group getColision(item& colider);
	group getClicked(int x, int y);
	group getDrags();
	item* getItem(int index);
	std::vector <item*> getItems();
	virtual void load(std::string fileName);
	virtual void save(std::string fileName);
	std::vector <SDL_Rect> getUpdateRegion();
	std::vector <SDL_Rect> getRects();
	void clearUpdateRegion();
	protected:
	std::vector <item*> items;
	std::vector <SDL_Rect> updateRects; 
};
class tile : public item
{
	public:
	void setState();
	void swapLayers(int first, int second);
	void add(item layer);
	virtual void draw(SDL_Surface * dest);
	virtual void drawLayer(SDL_Surface * dest, int depth);
	private:
	static int TileWidth, TileHeight; // default is 32X32
	std::vector <item> layers;
	void (*within)(); // code to run when user fully within
	void (*contact)(); // code to run when user makes contact at the boarders
	void (*mouseClick)(); // code to run when... Oh, you get the idea...
	void (*event)(SDL_Event);
};
class board : public item
{
	public:
	void setDimensions(int w, int h, tileSizeW, tileSizeH);
	virtual void draw(SDL_Surface * dest);
	void addItem(item toAdd, int x, int y);
	private:
	std::vector <item> tiles;
	SDL_Rect camera;
	int widthInTiles, heightInTiles, tileW, tileH;
};
#endif
