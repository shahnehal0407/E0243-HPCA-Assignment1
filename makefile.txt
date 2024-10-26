# Makefile to compile all .cpp files to .out files

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Source files
SRCS = $(wildcard *.cpp)

# Object files
OBJS = $(SRCS:.cpp=.out)

# Default target
all: $(OBJS)

# Rule to compile .cpp files to .out files
%.out: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Clean target to remove compiled files
clean:
	rm -f $(OBJS)

.PHONY: all clean

