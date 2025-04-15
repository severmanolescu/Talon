#pragma once

#include "Console.hpp"

#include <string>
#include "imgui.h"
#include "json.hpp"

class ConsolePanel {
public:
	void Render();

	void SaveSettings(nlohmann::json& root);
	void LoadSettings(const nlohmann::json& root);

private:
	bool show_info_ = true;
	bool show_warning_ = true;
	bool show_error_ = true;
	bool show_debug_ = true;
	bool auto_scroll_ = true;

	bool show_timestamp_ = true;

	int count_info_ = 0;
	int count_warn_ = 0;
	int count_error_ = 0;
	int count_debug_ = 0;

	char search_buffer_[128] = "";

	std::string ExtractFileName(const std::string& filePath);

	bool ShouldShowMessage(const ConsoleMessage& msg, ImVec4& out_color, const char*& out_tag);
	void DrawMessageContextMenu(const ConsoleMessage& msg);
	void DrawMessageCounts();
	void DrawLogMessage(const ConsoleMessage& msg, const ImVec4& color, const char* tag);
	void DrawUserInput();
};