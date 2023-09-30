all: release debug

release: build/release/tetris

build/release/tetris: main.cpp
	mkdir -p build/release
	clang++ -O3 main.cpp -o build/release/tetris -lsfml-graphics -lsfml-window -lsfml-system

#debug: main.cpp
#	clang++ -g main.cpp -o build/debug/tetris -lsfml-graphics -lsfml-window -lsfml-system 

debug: build/debug/tetris


build/debug/tetris: main.cpp
	mkdir -p build/debug
	clang++ -g main.cpp -o build/debug/tetris -lsfml-graphics -lsfml-window -lsfml-system 
	
compile: main.o

main.o: main.cpp
	clang++ -c main.cpp

link: tetris

tetris: main.o
	clang++ main.o -o tetris -lsfml-graphics -lsfml-window -lsfml-system

clean: 
	rm -f *.o tetris
	rm -rf build
