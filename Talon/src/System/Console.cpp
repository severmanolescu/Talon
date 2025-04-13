#include "Console.hpp"
#include <chrono>

std::vector<ConsoleMessage> Console::messages_;

std::string Console::GetCurrentTimestamp() {
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);

	std::tm tm{};
	localtime_s(&tm, &time);

	char buffer[9];
	std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm);
	return buffer;
}

void Console::LimitMessages() {
	if (messages_.size() > max_messages_) {
		messages_.erase(messages_.begin(), messages_.begin() + (messages_.size() - max_messages_));
	}
}

void Console::Info(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::INFO, message, file, line, GetCurrentTimestamp()});

	LimitMessages();
}

void Console::Warning(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::WARNING, message, file, line, GetCurrentTimestamp() });

	LimitMessages();
}

void Console::Error(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::ERROR, message, file, line, GetCurrentTimestamp() });

	LimitMessages();
}

void Console::Debug(const std::string& message, const char* file, int line) {
	messages_.push_back({ LogLevel::DEBUG, message, file, line, GetCurrentTimestamp() });

	LimitMessages();
}

const std::vector<ConsoleMessage>& Console::GetMessages(){
	return messages_;
}

void Console::Clear() {
	messages_.clear();
}
