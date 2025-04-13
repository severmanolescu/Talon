#include "HierarchyPanel.h"

#include "imgui.h"

void HierarchyPanel::Render(){
    ImGui::Begin("Hierarchy");

    ImGui::Text("Player");
    ImGui::Text("Camera");
    ImGui::Text("Wall");

    ImGui::End();
}
