all: 
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng risk.cpp sprite.cpp -o risk

OSFiles: 
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng OSFileTest.cpp sprite.cpp -o OSTest
	./OSTest

hints:
	./_ To compile everything by typing make then do this:
	./_ all: text test pong risk movie window

w2:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng openWindow2.cpp sprite.cpp -o window2
	./window2

text:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng textTest.cpp sprite.cpp -o text

test:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng Test_sprite.cpp sprite.cpp -o test
	
pong:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng gamePack/pong.cpp sprite.cpp -o pong

risk:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng gamePack/risk.cpp sprite.cpp -o risk
	cp -Rnv gamePack/res .
movie:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng Movie.cpp sprite.cpp -o movie

window:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng openWindow.cpp sprite.cpp -o windowTemplate

clean:
	rm pong movie risk test text windowTemplate

