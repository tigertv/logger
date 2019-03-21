#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <stdexcept>
#include "logger.h"

void threadFunc(Logger* log, int id) {
	log->info("Thread " + std::to_string(id) + " begins");

	// delay
	std::random_device rd;
	std::uniform_int_distribution<int> dist(2, 7);
	int delay =  dist(rd);
	std::this_thread::sleep_for(std::chrono::seconds(delay));

	std::uniform_int_distribution<int> dist2(0, (int)MessageType::ERROR);
	int msgType =  dist2(rd);
	log->message((MessageType)msgType, "Thread " +
										std::to_string(id) + " Result");
	log->info("Thread " + std::to_string(id) +
			  " ends (delay = " + std::to_string(delay) + ")");
}

int main(int argc, char **argv) {
	Logger *log;

	try {
		log = new Logger("myfile.log");
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	log->info("Main Thread Begins");

	std::vector<std::thread> threads;
	int numThreads = 15;

	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(threadFunc, log, i));
	}

	log->info("Main Thread Point 1");

	for(auto &t : threads){
		t.join();
	}

	log->info("Main Thread ends");
	delete log;

	return 0;
}
