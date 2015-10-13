CC = g++ -std=c++11
UTILSHPP = src/Utils.hpp
UTILSCPP = src/Utils.cpp
SHOTHPP = src/ShotSegmentation.hpp
SHOTCPP = src/ShotSegmentation.cpp
LIBS = `pkg-config --libs --cflags opencv`

all: FAST

Utils.o: $(UTILSHPP) $(UTILSCPP)
	@$(CC) -o src/$@ -c $(UTILSCPP)
	
ShotSegmentation.o: $(SHOTHPP) $(SHOTCPP)
	@$(CC) -o src/$@ -c $(SHOTCPP)

FAST: src/Utils.o src/ShotSegmentation.o src/main.cpp
	@$(CC) src/main.cpp src/Utils.o src/ShotSegmentation.o -o FAST $(LIBS)

clean:
	@rm -f FAST src/*.o
