#include "EditorUIManager.h"

#include <imgui.h>

void EditorUIManager::Render() {

    ImGui::DockSpaceOverViewport(ImGui::GetID("TalonDockspace"), ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

    ImGui::ShowDemoWindow();

    //hierarchy_panel_.Render();
    inspector_panel_.Render();
    //console_panel_.Render();
}
