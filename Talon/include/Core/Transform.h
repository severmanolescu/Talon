#pragma once

#include "MindCore.h"
#include "Vector2.h"

#include "memory"

class Transform : public MindCore {
public:
	Transform() {
		priority = 0;
	}

	Vector2 position_;

	Vector2 relative_position;

	Vector2 scale_ = { 1.0f, 1.0f };

	float rotation_ = .0f;

	Vector2 GetWorldPosition();

	void SetWorldPosition(const Vector2& world_pos);

	void DrawUI() override {
		ImGui::BeginChild("TransformChild", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 5.5f), true);
		
		ImGui::SeparatorText("Transform");

		DrawVector2Control("Position", position_);
		DrawVector2Control("Scale", scale_);
		
		ImGui::EndChild();
	}
};
