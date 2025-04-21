#include "BoxCollider.h"
#include "GameObject.h"
#include "WindowManager.h"

SDL_Rect BoxCollider::GetBounds() const {
	std::shared_ptr<Transform> transform = game_object_->GetTransform();

	if (!transform) return { 0, 0, 0, 0 };

	Vector2 world_pos = transform->GetWorldPosition();

	return SDL_Rect{
		static_cast<int>(world_pos.x + offset_.x),
		static_cast<int>(world_pos.y + offset_.y),
		width_,
		height_
	};
}

void BoxCollider::DrawGizmo() {
	if (!draw_debug_ || !WindowManager::GetRenderer()) return;

	SDL_Rect rect = GetBounds();
	SDL_SetRenderDrawColor(WindowManager::GetRenderer(), 0, 255, 0, 255);
	SDL_RenderDrawRect(WindowManager::GetRenderer(), &rect);
}

void BoxCollider::DrawUI(){
	BeginDraw("BoxCollider");

    DrawInt2Control(
        "Dimension",
        "Width", &width_,
        "Heihgt", &height_
    );

	DrawVector2Control("Offset", offset_);

	DrawCheckbox("Draw Debug", &draw_debug_);

	EndDraw();
}

void BoxCollider::Serialize(nlohmann::json& json){
	nlohmann::json box_collider;

	box_collider["type"] = "BoxCollider";

	box_collider["data"]["scale"] =
	{
		{"width", width_},
		{"height", height_}
	};

	box_collider["data"]["offset"] =
	{
		{"x", offset_.x},
		{"y", offset_.y}
	};

	box_collider["data"]["draw debug"] = draw_debug_;

	box_collider["active"] = active_;

	json.push_back(box_collider);
}

void BoxCollider::Deserialize(const nlohmann::json& json){
	if (json.contains("scale"))
	{
		width_ = json["scale"]["width"];
		height_ = json["scale"]["height"];
	}

	if (json.contains("offset")) {
		offset_ =
		{
			json["offset"]["x"],
			json["offset"]["y"]
		};
	}

	if (json.contains("draw debug")) {
		draw_debug_ = json["draw debug"];
	}
}

void BoxCollider::Awake(){
	transform_ = game_object_->GetTransform();
}
