#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <atomic>
#include <queue>
#include <boost/lockfree/queue.hpp>
#include <thread>

enum class MessageType {INFO, WARNING, ERROR};

class Logger {
	std::ofstream fs;
	std::string getDateTime();
	boost::lockfree::queue<std::string*> myqueue;
	//std::queue<std::string*> myqueue;
	std::thread worker;
	std::atomic_bool done;
	void work();
	void write(std::string* text);
public:
	Logger(std::string filePath);
	~Logger();
	void message(MessageType type, std::string text);
	void warning(std::string text);
	void error(std::string text);
	void info(std::string text);
};

#endif
