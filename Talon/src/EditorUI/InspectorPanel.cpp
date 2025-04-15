#include "InspectorPanel.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"

#include "MindEngine.h"

#include "imgui.h"

void InspectorPanel::Render(std::shared_ptr<GameObject> selected_object){
	ImGui::Begin("Inspector");

	if (!selected_object) {
		ImGui::Text("No object selected.");
		ImGui::End();
		return;
	}

	set_active_ = selected_object->IsActive();

    ImGui::SameLine();
    ImGui::Checkbox(" ", &set_active_);

    ImGui::SameLine();

    if (selected_object != last_selected_object_) {
        strncpy_s(rename_buffer_, IM_ARRAYSIZE(rename_buffer_), selected_object->name_.c_str(), _TRUNCATE);
        rename_buffer_[IM_ARRAYSIZE(rename_buffer_) - 1] = '\0';
        last_selected_object_ = selected_object;
    }

    if (ImGui::InputTextWithHint("##ObjectName", "Object name...", rename_buffer_, IM_ARRAYSIZE(rename_buffer_), ImGuiInputTextFlags_EnterReturnsTrue)) {
        selected_object->name_ = rename_buffer_;

		rename_buffer_[0] = '\0'; // Clear the buffer after renaming
    }

	std::vector<std::shared_ptr<MindCore>> components = selected_object->GetAllComponents();

	for (auto& component : components) {
		component->DrawUI();
	}

	selected_object->SetActive(set_active_);

	ImGui::End();
}
