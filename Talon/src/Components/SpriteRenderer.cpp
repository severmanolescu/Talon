#include "SpriteRenderer.h"
#include "GameObject.h"


void SpriteRendered::Awake(){
	transform_ = game_object_->GetTransform();
}

void SpriteRendered::Update() {
	if (!transform_ || !renderer_) {
		return;
	}

	SDL_Rect rect;

	rect.x = static_cast<int>(transform_->position_.x);
	rect.y = static_cast<int>(transform_->position_.y);

	rect.w = width_;
	rect.h = height_;

	SDL_SetRenderDrawColor(renderer_, color_r_, color_g_, color_b_, 255);
	SDL_RenderFillRect(renderer_, &rect);
}
