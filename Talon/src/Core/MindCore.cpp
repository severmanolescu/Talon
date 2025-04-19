#include "MindCore.h"
#include "GameObject.h"

void MindCore::DrawGizmo() {}

std::shared_ptr<MindCore> MindCore::Create() { return nullptr; }

void MindCore::BeginDraw(std::string component_name) {
    std::string frame_name = component_name + "Child";

    ImGui::BeginChild(
        frame_name.c_str(),
        ImVec2(0, ImGui::GetFrameHeightWithSpacing() * ui_frame_height_),
        true
    );

    ImGui::Checkbox(" ", &active_);
    ImGui::SameLine();
    ImGui::Text(component_name.c_str());

    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 20);
    if (ImGui::Button("X", ImVec2(20, 0))) {
        for (auto& comp : game_object_->GetAllComponents()) {
            if (comp.get() == this) {
                game_object_->RemoveComponent(comp);
                break;
            }
        }
    }
    ImGui::Separator();
}
