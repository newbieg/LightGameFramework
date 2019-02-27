all: 
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lpng risk.cpp sprite.cpp -o risk

hints:
	./_ To compile everything by typing make then do this:
	./_ all: text test pong risk movie window

text:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lpng textTest.cpp sprite.cpp -o text

test:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lpng Test_sprite.cpp sprite.cpp -o test
	
pong:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lpng gamePack/pong.cpp sprite.cpp -o pong

risk:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lpng gamePack/risk.cpp sprite.cpp -o risk
	cp -Rnv gamePack/res .
movie:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lpng Movie.cpp sprite.cpp -o movie

window:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lpng openWindow.cpp sprite.cpp -o windowTemplate

clean:
	rm pong movie risk test text windowTemplate

