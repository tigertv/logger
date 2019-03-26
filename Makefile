CXX = g++
FLAGS = -std=c++17 -Wall -g 
#SRC_DIR = src
#SRC = $(wildcard $(SRC_DIR)/*.cpp)
BOOST = -lboost_atomic -lboost_system -lboost_thread
SRC = $(wildcard *.cpp)
#eBIN_DIR = build
TESTS = test
OBJ := $(SRC:.cpp=.o)

.PHONY: all clean test-run

all: liblogger.so

liblogger.so: logger.so.o
	$(CXX) $(FLAGS) -shared -fPIC $< $(BOOST) -o $@ 

%.so.o: %.cpp
	$(CXX) $(FLAGS) -c -fPIC $^ -o $@ 

%.o: %.cpp 
	$(CXX) $(FLAGS) -c $< -o $@

test: test.cpp liblogger.so
	$(CXX) -I. -L. $(FLAGS) $< -Wl,-rpath=. -llogger -pthread -lboost_unit_test_framework -o $@
test-run: test
	./$<
clean: 
	rm -rf *.o *.so $(TESTS)
