#pragma once

#include "vector"
#include "string"

/**
* @brief Logs an informational message with file and line info.
*/
#define LOG_INFO(msg)    Console::Info(msg, __FILE__, __LINE__)

/**
* @brief Logs a warning message with file and line info.
*/
#define LOG_WARNING(msg) Console::Warning(msg, __FILE__, __LINE__)

/**
* @brief Logs an error message with file and line info.
*/
#define LOG_ERROR(msg)   Console::Error(msg, __FILE__, __LINE__)

/**
* @brief Logs a debug message with file and line info.
*/
#define LOG_DEBUG(msg)   Console::Debug(msg, __FILE__, __LINE__)

/**
* @brief Enum representing log severity levels.
*/
enum class LogLevel {
	INFO,	///< Informational log.
	WARNING,///< Warning log.
	ERROR,	///< Error log.
	DEBUG	///< Debug log.
};
/**
 * @brief Struct containing a single log message's details.
 */
struct ConsoleMessage {
	LogLevel level;          ///< Severity level of the message.

	std::string message;     ///< Log text content.
	std::string file;        ///< Source file name.

	int line;                ///< Line number in the file.

	std::string timestamp;   ///< Timestamp of when the log was generated.
};

/**
 * @brief Static class used to log and manage console messages.
 */
class Console {
public:
	/**
	 * @brief Logs an informational message.
	 */
	static void Info(const std::string& message, const char* file, int line);

	/**
	 * @brief Logs a warning message.
	 */
	static void Warning(const std::string& message, const char* file, int line);

	/**
	 * @brief Logs an error message.
	 */
	static void Error(const std::string& message, const char* file, int line);

	/**
	 * @brief Logs a debug message.
	 */
	static void Debug(const std::string& message, const char* file, int line);

	/**
	 * @brief Returns all logged messages.
	 */
	static const std::vector<ConsoleMessage>& GetMessages();

	/**
	 * @brief Clears all stored console messages.
	 */
	static void Clear();

private:
	static constexpr int max_messages_ = 2000; ///< Maximum number of stored messages.

	static std::vector<ConsoleMessage> messages_; ///< Internal message buffer.

	/**
	 * @brief Gets the current timestamp formatted as a string.
	 */
	static std::string GetCurrentTimestamp();

	/**
	 * @brief Ensures the log does not exceed the message limit.
	 */
	static void LimitMessages();
};