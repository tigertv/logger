#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "logger.h"

Logger::Logger(std::string filePath) : 
					myqueue(128), 
					//worker(&Logger::work, this),
					done(false) {
	this->fs.open(filePath, std::ofstream::trunc | std::ofstream::out);
	if (!this->fs.is_open()) throw std::runtime_error("Can't open the file");
	this->worker = std::thread(&Logger::work, this);
}

Logger::~Logger() {
	this->done = true;
	this->worker.join();
	this->fs.close();
}

void Logger::write(std::string* text) {
	if (text == nullptr) return;

	std::cout << *text << std::endl;
	this->fs << *text << std::endl;
	delete text;
}

/*
void Logger::work() {
	std::string *value;
	while (!this->done) {
		while (value = this->myqueue.front()) {
			this->myqueue.pop();
			this->write(value);
		}
	}

	while (value = this->myqueue.front()) {
		this->myqueue.pop();
		this->write(value);
	}
}
//*/

//*
void Logger::work() {
	std::string *value;
	while (!this->done) {
		while (this->myqueue.pop(value)) {
			this->write(value);
		}
	}

	while (this->myqueue.pop(value)) {
		this->write(value);
	}
}
//*/

void Logger::message(MessageType type, std::string text) {
	std::string sType;
	switch(type) {
		case MessageType::WARNING:
			sType = "WARN";
			break;
		case MessageType::INFO:
			sType = "INFO";
			break;
		case MessageType::ERROR:
			sType = "ERR ";
			break;
		default:
			sType = "UNKN";
	}
			
	std::string *output = new std::string(this->getDateTime() +
			" - " + sType + " - " + text);
	this->myqueue.push(output);
}

void Logger::warning(std::string text) {
	this->message(MessageType::WARNING, text);
}

void Logger::error(std::string text) {
	this->message(MessageType::ERROR, text);
}

void Logger::info(std::string text) {
	this->message(MessageType::INFO, text);
}

std::string Logger::getDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&tm, "%c %Z");
	return ss.str();
}
