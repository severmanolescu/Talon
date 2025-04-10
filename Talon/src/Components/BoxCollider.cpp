#include "BoxCollider.h"
#include "GameObject.h"

SDL_Rect BoxCollider::GetBounds() const {
	if (!transform_) return { 0, 0, 0, 0 };

	return SDL_Rect{
		static_cast<int>(transform_->position_.x),
		static_cast<int>(transform_->position_.y),
		width_,
		height_
	};
}

void BoxCollider::Awake(){
	transform_ = game_object_->GetTransform();
}
