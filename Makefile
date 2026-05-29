# Linux Makefile for Physics Collision Simulator

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -Isrc $(shell pkg-config --cflags sdl2 SDL2_image 2>/dev/null)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image 2>/dev/null || echo "-lSDL2 -lSDL2_image")

# Source files
SRCS = src/main.cpp \
       src/core/config.cpp \
       src/core/core.cpp \
       src/core/tick.cpp \
       src/entity/cannonball.cpp \
       src/entity/rope.cpp \
       src/math/vector_math.cpp \
       src/ui/screen.cpp \
       src/ui/toolbar.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = physics-simulation

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Generic rule for compiling .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
