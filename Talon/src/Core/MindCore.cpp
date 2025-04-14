#include "MindCore.h"
#include "GameObject.h"

#include <imgui_internal.h>

void MindCore::Awake() {}
void MindCore::Start() {}
void MindCore::Update() {}
void MindCore::OnDestroy() {}
void MindCore::DrawGizmo() {}
void MindCore::DrawUI(){}

void MindCore::BeginDraw(std::string component_name){
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

void MindCore::EndDraw(){
	ImGui::EndChild();
}

void MindCore::DrawCheckbox(const std::string& label, bool* value)
{
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::Checkbox("##Checkbox", value);

    ImGui::PopID();
}

void MindCore::DrawIntControl(const std::string& label, int* value)
{
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::Separator();

    ImGui::SetNextItemWidth(ImGui::CalcItemWidth());
    ImGui::InputInt("##Int", value, 0, 0);

    ImGui::PopID();
}

void MindCore::DrawFloatControl(const std::string& label, float* value)
{

    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::Separator();

    ImGui::SetNextItemWidth(ImGui::CalcItemWidth());
    ImGui::InputFloat("##Float", value, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal);

    ImGui::PopID();
}

void MindCore::DrawInt2Control(
    const std::string& label,
    const std::string& first_value_label,
    int* first_value,
    const std::string& second_value_label,
    int* second_value,
    float columnWidth)
{
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
	ImGui::Separator();

    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

    ImGui::Text(first_value_label.c_str()); ImGui::SameLine();
    ImGui::InputInt("##X", first_value, 0, 0);
    ImGui::PopItemWidth();

    ImGui::SameLine();

    ImGui::Text(second_value_label.c_str()); ImGui::SameLine();
    ImGui::InputInt("##Y", second_value, 0, 0);
    ImGui::PopItemWidth();

    ImGui::PopID();
}

void MindCore::DrawVector2Control(
    const std::string& label, 
    Vector2& values, 
    float resetValue, 
    float columnWidth)
{
    ImGui::PushID(label.c_str());

    ImGui::Text("%s", label.c_str());
    ImGui::Separator();

    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

    ImGui::Text("X"); ImGui::SameLine();
    ImGui::InputFloat("##X", &values.x, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal);
    ImGui::SameLine();

    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::InputFloat("##Y", &values.y, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_CharsDecimal);

    ImGui::PopItemWidth();
    ImGui::PopItemWidth();

    ImGui::PopID();
}
