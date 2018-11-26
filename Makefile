
all:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lpng Test_sprite.cpp sprite.cpp lines.cpp
	
pong:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image gamePack/pong.cpp sprite.cpp

risk:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image gamePack/risk.cpp sprite.cpp
	cp -Rnv gamePack/res .
	
	
window:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image openWindow.cpp sprite.cpp
