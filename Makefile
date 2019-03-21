APP = main
CXX = g++
FLAGS = -std=c++17 -Wall -g 
#SRC_DIR = src
#SRC = $(wildcard $(SRC_DIR)/*.cpp)
BOOST = -lboost_atomic -lboost_system -lboost_thread
SRC = $(wildcard *.cpp)
#eBIN_DIR = build
OBJ := $(SRC:.cpp=.o)

.PHONY: all clean 

all: main

main: main.o liblogger.so
	$(CXX) -I. -L. $(FLAGS) $< -o $@ -Wl,-rpath=. -llogger -pthread 

liblogger.so: logger.so.o
	$(CXX) $(FLAGS) -shared -fPIC $< -o $@

%.so.o: %.cpp
	$(CXX) $(FLAGS) -c -fPIC $^ -o $@
	# $(CXX) $(FLAGS) -c -fPIC $^ -o $@ $(BOOST)

%.o: %.cpp 
	$(CXX) $(FLAGS) -c $< -o $@

clean: 
	rm -rf $(APP) *.o *.so
