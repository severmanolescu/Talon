#pragma once

#include "MindCore.h"
#include "Transform.h"
#include "GameObject.h"

#include <SDL2/SDL.h>

#include "memory"

class BoxCollider : public MindCore {
public:
	int width_ = 50;
	int height_ = 50;

	SDL_Rect GetBounds() const {
		if (!transform_) return { 0, 0, 0, 0 };

		return SDL_Rect{
			static_cast<int>(transform_->position_.x),
			static_cast<int>(transform_->position_.y),
			width_,
			height_
		};
	}

	void Awake() override {
		transform_ = game_object_->GetTransform();
	}

private:
	std::shared_ptr<Transform> transform_;
};
