#include "Console.hpp"

std::vector<ConsoleMessage> Console::messages_;

void Console::Info(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::INFO, message, file, line });
}

void Console::Warning(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::WARNING, message, file, line });
}

void Console::Error(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::ERROR, message, file, line });
}

void Console::Debug(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::DEBUG, message, file, line });
}

const std::vector<ConsoleMessage>& Console::GetMessages(){
	return messages_;
}

void Console::Clear() {
	messages_.clear();
}