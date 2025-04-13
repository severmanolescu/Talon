#pragma once

#include "InspectorPanel.h"
#include "ConsolePanel.h"
#include "HierarchyPanel.h"

#include "WindowManager.h"

class EditorUIManager {
public:
	void RenderPanels();

	void RenderImGui(SDL_Renderer* render);

	void InitFrame();

	void InitImGui(SDL_Window* window, SDL_Renderer* renderer);

	void SaveSettings(const std::string& file_path);

	void LoadSettings(const std::string& file_path);

private:
	HierarchyPanel hierarchy_panel_;
	ConsolePanel console_panel_;
	InspectorPanel inspector_panel_;
};