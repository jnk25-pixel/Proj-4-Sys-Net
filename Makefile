CXX = c++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -Wpedantic
LDLIBS =

.PHONY: all clean

all: matrix

matrix: matrix.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)

clean:
	$(RM) matrix
