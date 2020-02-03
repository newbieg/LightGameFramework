w2:
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lpng buttonBuilder.cpp sprite.cpp -o ButtonBuilder
	g++ -lSDL2  installPath.cpp -o installPath

install:
	# copying font files to `installPath`
	cp -r res `./installPath`
	# copy ButtonBuilder to your bin, the following line would do that
	cp ButtonBuilder ~/bin

