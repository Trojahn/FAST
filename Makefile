CPPFLAGS := -O2 -g -std=c++11 -pthread -Wl,--no-as-needed
LDLIBS := `pkg-config --libs --cflags opencv`
OBJS = Utils.o ShotSegmentation.o
PROG = FAST
CXX = g++

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) src/main.cpp -o $@ $(LDLIBS)

Utils.o: src/Utils.cpp src/Utils.hpp
	$(CXX) $(CPPFLAGS) -c src/Utils.cpp -o $@
	
ShotSegmentation.o: src/ShotSegmentation.cpp src/ShotSegmentation.hpp
	$(CXX) $(CPPFLAGS) -c src/ShotSegmentation.cpp -o $@
	
clean:
	@rm -f $(PROG) *.o
