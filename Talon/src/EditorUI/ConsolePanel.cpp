#include "ConsolePanel.h"
#include "Console.hpp"

#include "imgui.h"
#include <filesystem>

std::string ConsolePanel::ExtractFileName(const std::string& filePath)
{
    return std::filesystem::path(filePath).filename().string();
}

void ConsolePanel::Render() {
    ImGui::Begin("Console");

    if (ImGui::Button("Clear")) {
        Console::Clear();
    }

    for (const auto& msg : Console::GetMessages()) {
        std::string file = ExtractFileName(msg.file);
        switch (msg.level) {
        case LogLevel::INFO:
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "[Info] %s (%s, %d)", msg.message.c_str(), file.c_str(), msg.line);
            break;
        case LogLevel::WARNING:
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "[Warning] %s (%s, %d)", msg.message.c_str(), file.c_str(), msg.line);
            break;
        case LogLevel::ERROR:
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "[Error] %s (%s, %d)", msg.message.c_str(), file.c_str(), msg.line);
            break;
        case LogLevel::DEBUG:
            ImGui::TextColored(ImVec4(.4f, .6f, 1, 1), "[Debug] %s (%s, %d)", msg.message.c_str(), file.c_str(), msg.line);
            break;
        }
    }

    ImGui::End();
}
