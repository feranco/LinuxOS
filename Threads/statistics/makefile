# compiler
CXX=g++
CXXFLAGS=-g -std=c++14 -Wall -Werror 
BIN=statistics

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^ -lpthread

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)
