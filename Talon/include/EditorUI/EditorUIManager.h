#pragma once

#include "InspectorPanel.h"
#include "ConsolePanel.h"
#include "HierarchyPanel.h"
#include "ScenePanel.hpp"

#include "WindowManager.h"

class EditorUIManager {
public:
	void RenderPanels();

	void RenderImGui();

	void InitFrame();

	void InitImGui();

	void SaveSettings(const std::string& file_path);

	void LoadSettings(const std::string& file_path);

private:
	HierarchyPanel hierarchy_panel_;
	ConsolePanel console_panel_;
	InspectorPanel inspector_panel_;
	ScenePanel scene_panel_;
};