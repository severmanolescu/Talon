#include "BoxCollider.h"
#include "GameObject.h"

SDL_Rect BoxCollider::GetBounds() const {
	if (!transform_) return { 0, 0, 0, 0 };

	return SDL_Rect{
		static_cast<int>(transform_->position_.x + offset_.x),
		static_cast<int>(transform_->position_.y + offset_.y),
		width_,
		height_
	};
}

void BoxCollider::DrawDebug(SDL_Renderer* renderer) {
	if (!draw_debug_ || !renderer || !transform_) return;

	SDL_Rect rect = GetBounds();
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
}

void BoxCollider::DrawUI(){
	BeginDraw("BoxCollider");

    DrawInt2Control(
        "Dimension",
        "Width",
        &width_,
        "Heihgt",
        &height_
    );

	DrawVector2Control("Offset", offset_);

	DrawCheckbox("Draw Debug", &draw_debug_);

	EndDraw();
}

void BoxCollider::Awake(){
	ui_frame_height_ = 6.6f;

	transform_ = game_object_->GetTransform();
}
