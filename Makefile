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

all: liblogger.so

main: main.o liblogger.so
	$(CXX) -I. -L. $(FLAGS) $< -o $@ -Wl,-rpath=. -llogger -pthread 

liblogger.so: logger.so.o
	$(CXX) $(FLAGS) -shared -fPIC $< $(BOOST) -o $@ 

%.so.o: %.cpp
	$(CXX) $(FLAGS) -c -fPIC $^ -o $@ 

%.o: %.cpp 
	$(CXX) $(FLAGS) -c $< -o $@

test: test.cpp liblogger.so
	$(CXX) -I. -L. $(FLAGS) $< -Wl,-rpath=. -llogger -pthread -lboost_unit_test_framework -o $@

#test1: test1.cpp
#	$(CXX) $(FLAGS) $< -o $@
	
#test2: test2.cpp
#	$(CXX) $(FLAGS) $< -o $@

#test3: test3.cpp liblogger.so
#	$(CXX) -I. -L. $(FLAGS) $< -o $@ -Wl,-rpath=. -llogger -pthread 

clean: 
	rm -rf $(APP) *.o *.so test
