CPPFLAGS := -O2 -std=c++11 -pthread -Wl,--no-as-needed
LDLIBS := `pkg-config --libs --cflags opencv4`
OBJS = Utils.o ShotSegmentation.o
PROG = FAST
CXX = g++

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) src/main.cpp -o $@ $(LDLIBS)

%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) -c -o $@ $< $(LDLIBS)
	
clean:
	@rm -f $(PROG) *.o
