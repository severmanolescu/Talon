#pragma once

#include "vector"
#include "string"

#define LOG_INFO(msg)    Console::Info(msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Console::Warning(msg, __FILE__, __LINE__)
#define LOG_ERROR(msg)   Console::Error(msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg)   Console::Debug(msg, __FILE__, __LINE__)

enum class LogLevel {
	INFO,
	WARNING,
	ERROR,
	DEBUG
};

struct ConsoleMessage {
	LogLevel level;

	std::string message;

	std::string file;
	int line;

	std::string timestamp;
};

class Console {
public:
	static void Info(const std::string& message, const char* file, int line);
	static void Warning(const std::string& message, const char* file, int line);
	static void Error(const std::string& message, const char* file, int line);
	static void Debug(const std::string& message, const char* file, int line);

	static const std::vector<ConsoleMessage>& GetMessages();

	static void Clear();
private:
	const static int max_messages_ = 2000;

	static std::vector<ConsoleMessage> messages_;

	static std::string GetCurrentTimestamp();

	static void LimitMessages();
};