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

	// delay
	std::random_device rd;
	std::uniform_int_distribution<int> dist(2, 7);
	int delay =  dist(rd);
	std::this_thread::sleep_for(std::chrono::seconds(delay));

	// each thread produces 100000 messages
	for (int i = 0;i < 100000 - 2; i++) {
		MessageType msgType = getRandomMsgType();
		log->message(msgType, "Thread " +
								std::to_string(id) + " Result");
	}

	log->info("Thread " + std::to_string(id) +
			  " ends (delay = " + std::to_string(delay) + ")");
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

/*
BOOST_AUTO_TEST_CASE( my_test )
{
    // seven ways to detect and report the same error:
    BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error

    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error

    if( add( 2,2 ) != 4 )
      BOOST_ERROR( "Ouch..." );            // #3 continues on error

    if( add( 2,2 ) != 4 )
      BOOST_FAIL( "Ouch..." );             // #4 throws on error

    if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error

    BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
                         "add(..) result: " << add( 2,2 ) );

    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error
}


//*/
