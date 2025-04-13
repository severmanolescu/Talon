#include "ConsolePanel.h"
#include "Console.hpp"

#include <filesystem>

std::string ConsolePanel::ExtractFileName(const std::string& filePath){
    return std::filesystem::path(filePath).filename().string();
}

bool ConsolePanel::ShouldShowMessage(const ConsoleMessage& message, ImVec4& out_color, const char*& out_tag){
	switch (message.level) {
		case LogLevel::INFO: {
			if (!show_info_) return false;
			out_color = ImVec4(0.4f, 0.8f, 1.0f, 1.0f);
			out_tag = "[Info]";

			count_info_++;

			break;
		}
		case LogLevel::WARNING: {
			if (!show_warning_) return false;

			out_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
			out_tag = "[Warning]";

			count_warn_++;

			break;
		}
		case LogLevel::ERROR: {
			if (!show_error_) return false;

			out_color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
			out_tag = "[Error]";

			count_error_++;

			break;
		}
		case LogLevel::DEBUG: {
			if (!show_debug_) return false;

			out_color = ImVec4(0.5f, 0.5f, 1.0f, 1.0f);
			out_tag = "[Debug]";

			count_debug_++;

			break;
		}
	}

	return true;
}

void ConsolePanel::DrawMessageContextMenu(const ConsoleMessage& message){
	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::Selectable("Copy Message")) {
			ImGui::SetClipboardText(message.message.c_str());
		}

		if (ImGui::Selectable("Copy File + Line")) {
			std::string file_line = message.file + ":" + std::to_string(message.line);
			ImGui::SetClipboardText(file_line.c_str());
		}

		if (ImGui::Selectable("Open in Visual Studio")) {
			std::string command = "devenv.exe /Edit \"" + message.file + "\"";
			std::system(command.c_str());
		}

		ImGui::EndPopup();
	}
}

void ConsolePanel::DrawMessageCounts(){
	ImGui::BeginChild("LogCounts", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Separator();

	ImGui::Text("Info [%d]", count_info_); ImGui::SameLine();
	ImGui::Text("Warning [%d]", count_warn_); ImGui::SameLine();
	ImGui::Text("Error [%d]", count_error_); ImGui::SameLine();
	ImGui::Text("Debug [%d]", count_debug_);

	ImGui::EndChild();
}

void ConsolePanel::DrawLogMessage(const ConsoleMessage& message, const ImVec4& color, const char* tag){
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

	ImGui::BeginChild(
		("msg" + std::to_string(&message - &Console::GetMessages()[0])).c_str(),
		ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 2.0f),
		true,
		ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGui::Text("%s", tag);
	ImGui::PopStyleColor();

	if (show_timestamp_) {
		ImGui::SameLine();
		ImGui::Text("[%s]", message.timestamp.c_str());
	}

	ImGui::SameLine();
	ImGui::Text("%s (%s, %d)", message.message.c_str(), message.file.c_str(), message.line);

	// TO-DO: Add the feature for the editor to open the file at the line number
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
		std::string command = "devenv.exe /Edit \"" + message.file + "\"";
		std::system(command.c_str());
	}

	ImGui::EndChild();

	DrawMessageContextMenu(message);

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
}

void ConsolePanel::DrawUserInput(){
	if (ImGui::Button("Clear")) {
		Console::Clear();
	}

	ImGui::SameLine();
	ImGui::Checkbox("Info", &show_info_);

	ImGui::SameLine();
	ImGui::Checkbox("Warning", &show_warning_);

	ImGui::SameLine();
	ImGui::Checkbox("Error", &show_error_);

	ImGui::SameLine();
	ImGui::Checkbox("Debug", &show_debug_);

	ImGui::SameLine();
	ImGui::Checkbox("Auto-scroll", &auto_scroll_);

	ImGui::SameLine();
	ImGui::Checkbox("Show Timestamp", &show_timestamp_);

	ImGui::SameLine();
	search_buffer_[0] = '\0';
	ImGui::InputTextWithHint("##Search", "Search logs...", search_buffer_, IM_ARRAYSIZE(search_buffer_));

	ImGui::Separator();
}

void ConsolePanel::Render() {
	count_info_ = count_warn_ = count_error_ = count_debug_ = 0;

    ImGui::Begin("Console");

	DrawUserInput();

	ImGui::BeginChild("ConsoleScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), false);

	std::string search_string = search_buffer_;
	std::transform(search_string.begin(), search_string.end(), search_string.begin(), ::tolower);

	for (const auto& msg : Console::GetMessages()) {
		std::string log_text = msg.message;
		std::transform(log_text.begin(), log_text.end(), log_text.begin(), ::tolower);

		if (!search_string.empty() && log_text.find(search_string) == std::string::npos)
			continue;

		ImVec4 color;
		const char* tag = "";

		bool dont_print = false;

		if (!ShouldShowMessage(msg, color, tag)){
			continue;
		}

		DrawLogMessage(msg, color, tag);
	}

	if (auto_scroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0f) {
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::EndChild(); // ConsoleScrollRegion

	DrawMessageCounts();

    ImGui::End(); // End Console
}

void ConsolePanel::SaveSettings(nlohmann::json& root){
	root["console"]["show_info"] = show_info_;
	root["console"]["show_warning"] = show_warning_;
	root["console"]["show_error"] = show_error_;
	root["console"]["show_debug"] = show_debug_;
	root["console"]["show_timestamps"] = show_timestamp_;
}

void ConsolePanel::LoadSettings(const nlohmann::json& root) {
	if (root.contains("console")) {
		show_info_ = root["console"].value("show_info", true);
		show_warning_ = root["console"].value("show_warning", true);
		show_error_ = root["console"].value("show_error", true);
		show_debug_ = root["console"].value("show_debug", true);
		show_timestamp_ = root["console"].value("show_timestamps", true);
	}
}
