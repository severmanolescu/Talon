#pragma once

#include "InspectorPanel.h"
#include "ConsolePanel.h"
#include "HierarchyPanel.h"

class EditorUIManager {
public:
	void Render();

private:
	HierarchyPanel hierarchy_panel_;
	ConsolePanel console_panel_;
	InspectorPanel inspector_panel_;
};