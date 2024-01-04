all: release debug

release: build/release/tetris

build/release/tetris: main.cpp build/release/images
	mkdir -p build/release
	clang++ -O3 main.cpp -o build/release/tetris -lsfml-graphics -lsfml-window -lsfml-system

build/release/images:
	mkdir -p ./build/release/images
	cp ./images/* ./build/release/images/

debug: build/debug/tetris

build/debug/tetris: main.cpp build/debug/images
	mkdir -p build/debug
	clang++ -g main.cpp -o build/debug/tetris -lsfml-graphics -lsfml-window -lsfml-system 

build/debug/images:
	cp -r ./images ./build/debug

compile: main.o

main.o: main.cpp
	clang++ -c main.cpp

link: tetris

tetris: main.o
	clang++ main.o -o tetris -lsfml-graphics -lsfml-window -lsfml-system

clean: 
	rm -f *.o tetris
	rm -rf build

install: release
	mkdir -p /usr/local/sfml-games
	cp -rf "./build/release" /usr/local/sfml-games/tetris
	ln -fs "/usr/local/sfml-games/tetris/tetris" /usr/local/bin
