#include "InspectorPanel.h"
#include "imgui.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h" // or Rigidbody2D, etc.

void InspectorPanel::SetSelectedObject(std::shared_ptr<GameObject> object) {
	selected_ = object;
}

void InspectorPanel::Render() {
	ImGui::Begin("Inspector");

	if (!selected_) {
		ImGui::Text("No object selected.");
		ImGui::End();
		return;
	}

	ImGui::Text("Name: %s", selected_->name_.c_str());

	// --- Transform Component ---
	auto transform = selected_->GetComponent<Transform>();
	if (transform) {
		ImGui::SeparatorText("Transform");
		ImGui::InputFloat2("Position", &transform->position_.x);
		ImGui::InputFloat2("Scale", &transform->scale_.x);
	}

	// --- Rigidbody Component ---
	auto rb = selected_->GetComponent<Rigidbody>();
	if (rb) {
		ImGui::SeparatorText("Rigidbody");
		ImGui::InputFloat2("Velocity", &rb->velocity_.x);
		ImGui::Checkbox("Use Gravity", &rb->use_gravity_);
		ImGui::InputFloat("Mass", &rb->mass_);
	}

	ImGui::End();
}
