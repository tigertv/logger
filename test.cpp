#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LoggerTestModule
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <stdexcept>
#include "logger.h"

BOOST_AUTO_TEST_SUITE(LoggerTestSuite)

int getFileLines(std::string filePath) {
	// check lines
	int count = 0;
	std::string line;
	std::ifstream file(filePath);
	while (getline(file, line))
		count++;
	return count;
}

MessageType getRandomMsgType() {
	std::random_device rd;
	std::uniform_int_distribution<int> dist2(0, (int)MessageType::ERROR);
	return (MessageType)dist2(rd);
}

void threadFunc(Logger* log, int id) {
	log->info("Thread " + std::to_string(id) + " begins");

	// each thread produces 100000 messages
	for (int i = 0;i < 100000 - 2; i++) {
		MessageType msgType = getRandomMsgType();
		log->message(msgType, 
			"Thread " +	std::to_string(id) + " Result from message " + std::to_string(i));
	}

	log->info("Thread " + std::to_string(id) + " ends ");
}

BOOST_AUTO_TEST_CASE(logger_test1) {
	BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_CASE(logger_test2)
{
	const std::string filePath = "myfile.log";
	Logger *log;
	try {
		log = new Logger(filePath);
	} catch (const std::exception& e) {
		BOOST_FAIL( e.what() );
	}

	log->info("Main Thread Begins");

	std::vector<std::thread> threads;
	const int numThreads = 20;

	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(threadFunc, log, i));
	}

	log->info("Main Thread Point 1");

	for(auto &t : threads){
		t.join();
	}

	log->info("Main Thread ends");
	delete log;

	int count = getFileLines(filePath);
	BOOST_CHECK_EQUAL(count, 2000003);	
}

BOOST_AUTO_TEST_SUITE_END()
