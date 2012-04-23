CXXFLAGS=-pedantic -Wall -Wextra -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Werror -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code -g -O0

all: bin/include/persistent-data-structure.o src/include/persistent-segment-tree.h src/main.cpp
	g++ $(CXXFLAGS) bin/include/persistent-data-structure.o src/main.cpp -o bin/persistent-segment-tree

bin/include/persistent-data-structure.o: src/include/persistent-data-structure.h src/include/persistent-data-structure.cpp
	mkdir -p bin/include/
	g++ $(CXXFLAGS) src/include/persistent-data-structure.cpp -c -o bin/include/persistent-data-structure.o

clean:
	rm -rf bin/
