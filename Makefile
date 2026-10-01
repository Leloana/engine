CXX := g++

CXXFLAGS := $(shell sdl2-config --cflags) -ggdb3 -O0 -std=c++17 -Wall
LDLIBS   := $(shell sdl2-config --libs) -lSDL2_image -lm

HDRS :=
SRCS := main.cc
OBJS := $(SRCS:.cc=.o)
EXEC := engine

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(LDLIBS)

%.o: %.cc $(HDRS) Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
