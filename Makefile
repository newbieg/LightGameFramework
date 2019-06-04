all: w2 text test pong risk

OSFiles: 
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/OSFileTest.cpp sprite.cpp -o OSTest
	./OSTest

hints:
	# To compile everything by typing make then do this:
	# all: text test pong risk movie window ...etc.
	# I don't know why I didn't think hash would work for comments in make...

w2:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/openWindow2.cpp sprite.cpp -o window2

text:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/textTest.cpp sprite.cpp -o text

test:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/Test_sprite.cpp sprite.cpp -o test
	
pong:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/pong.cpp sprite.cpp -o pong

risk:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/risk.cpp sprite.cpp -o risk
	cp -Rnv gamePack/res .
movie:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/Movie.cpp sprite.cpp -o movie

window:
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/openWindow.cpp sprite.cpp -o windowTemplate
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng demo/openWindow2.cpp sprite.cpp -o windowTemplate2

clean:
	rm pong movie risk test text windowTemplate window2

