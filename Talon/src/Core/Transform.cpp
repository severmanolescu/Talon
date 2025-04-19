#include "Transform.h"
#include "GameObject.h"

Vector2 Transform::GetWorldPosition() {
	if (auto parent = game_object_->parent_.lock()) {
		return position_ + parent->GetTransform()->GetWorldPosition();
	}

	return position_;
}

void Transform::SetWorldPosition(const Vector2& world_pos) {
    if (auto parent = game_object_->parent_.lock()) {
        auto parent_world = parent->GetTransform()->GetWorldPosition();
        position_ = world_pos - parent_world;
    }
    else {
        position_ = world_pos;
    }
}

void Transform::DrawUI()
{
	ImGui::BeginChild("TransformChild", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 5.5f), true);

	ImGui::SeparatorText("Transform");

	DrawVector2Control("Position", position_);
	DrawVector2Control("Scale", scale_);

	ImGui::EndChild();
}

void Transform::Serialize(nlohmann::json& json)
{
	nlohmann::json transform;

	transform["type"] = "Transform";

	transform["data"]["position"] =
	{
		{"x", position_.x}, 
		{"y", position_.y}
	};

	transform["data"]["relative position"] =
	{ 
		{"x", relative_position.x}, 
		{"y", relative_position.y} 
	};

	transform["data"]["scale"] =
	{ 
		{"x", scale_.x}, 
		{"y", scale_.y} 
	};

	transform["data"]["rotation"] = rotation_;

	transform["active"] = active_;

	json.push_back(transform);
}

void Transform::Deserialize(const nlohmann::json& json)
{
	if (json.contains("position"))
	{
		position_ =
		{
			json["position"]["x"],
			json["position"]["y"]
		};
	}

	if (json.contains("relative position"))
	{
		relative_position = 
		{ 
			json["relative position"]["x"], 
			json["relative position"]["y"] 
		};
	}

	if (json.contains("scale")) {
		scale_ = 
		{ 
			json["scale"]["x"], 
			json["scale"]["y"] 
		};
	}

	if (json.contains("rotation")) {
		rotation_ = json["rotation"];
	}
}
