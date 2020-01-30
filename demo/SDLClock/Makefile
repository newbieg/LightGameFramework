

all: 
	# main program is compiled
	g++ -O3 -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng myClock.cpp sprite.cpp -o Timer
	# Side program that shows where to install the file per SDL recommends.
	g++ -lSDL2 installPath.cpp -o installPath

install:
	# installs the program by doing the following:
	# copy resources to that path.
	cp -Ruv res `./installPath`
	# copy main program to local bin
	cp -Rnv Timer ~/bin


clean:
	rm Timer 


