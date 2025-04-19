#pragma once

#include "Console.hpp"

#include <string>
#include "imgui.h"
#include "json.hpp"

/// @addtogroup UI
/// @{
/**
 * @brief Panel for displaying log messages from the engine and user scripts.
 *
 * Includes filters, message type toggles, search functionality, auto-scroll,
 * and context menu actions (copy, clear, jump to file, etc.).
 */
class ConsolePanel {
public:
	/**
	 * @brief Render the Console UI panel.
	 */
	void Render();

	/**
	 * @brief Save current settings (filters, scroll state) to JSON.
	 *
	 * @param root The JSON root object to append settings to.
	 */
	void SaveSettings(nlohmann::json& root);

	/**
	 * @brief Load settings from a previously saved JSON.
	 *
	 * @param root The JSON object containing the saved settings.
	 */
	void LoadSettings(const nlohmann::json& root);

private:
	// Filter toggles
	bool show_info_ = true;
	bool show_warning_ = true;
	bool show_error_ = true;
	bool show_debug_ = true;
	bool auto_scroll_ = true;

	// Additional UI options
	bool show_timestamp_ = true;

	// Message type counters
	int count_info_ = 0;
	int count_warn_ = 0;
	int count_error_ = 0;
	int count_debug_ = 0;

	// Search box input
	char search_buffer_[128] = "";

	/**
	 * @brief Extracts the file name from a full file path.
	 *
	 * @param filePath Full file path (e.g., "src/Console.cpp")
	 * @return File name only (e.g., "Console.cpp")
	 */
	std::string ExtractFileName(const std::string& filePath);

	/**
	 * @brief Determines whether a message should be shown based on current filters.
	 *
	 * @param msg The message to evaluate.
	 * @param out_color Output parameter for the tag color.
	 * @param out_tag Output parameter for the tag label ("[Info]", "[Error]", etc.)
	 * @return true if the message passes filters and should be displayed.
	 */
	bool ShouldShowMessage(const ConsoleMessage& msg, ImVec4& out_color, const char*& out_tag);

	/**
	 * @brief Draws the right-click context menu for a message.
	 *
	 * @param msg The message to draw context for.
	 */
	void DrawMessageContextMenu(const ConsoleMessage& msg);

	/**
	 * @brief Draws counters for log types (Info [X], Warning [Y], etc.)
	 */
	void DrawMessageCounts();

	/**
	 * @brief Draws a single log message.
	 *
	 * @param msg The message to display.
	 * @param color Color of the tag label.
	 * @param tag Text tag (e.g., "[Info]")
	 */
	void DrawLogMessage(const ConsoleMessage& msg, const ImVec4& color, const char* tag);

	/**
	 * @brief Draws user input UI elements: search box, toggles, clear button, etc.
	 */
	void DrawUserInput();
};
/// @}
