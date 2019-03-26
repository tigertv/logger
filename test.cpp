#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <stdexcept>
#include "logger.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE( my_test )
{
	Logger *log;
	try {
		log = new Logger("myfile.log");
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		//return 1;
		exit(1);
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

}

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

//BOOST_AUTO_TEST_SUITE_END()

//*/
