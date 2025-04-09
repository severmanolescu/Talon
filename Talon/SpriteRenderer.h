#pragma once

#include "GameObject.h"
#include "MindCore.h"
#include "Transform.h"

#include <SDL2/SDL.h>

#include "memory"

class SpriteRendered : public MindCore {
public:
	SpriteRendered(SDL_Renderer* renderer) : renderer_(renderer) {}

	void Awake() override {
		transform_ = game_object_->GetTransform();
	}

	void Update() override {
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

	int width_ = 50;
	int height_ = 50;
	Uint8 color_r_ = 255, color_g_ = 255, color_b_ = 0;

private:
	std::shared_ptr<Transform> transform_;

	SDL_Renderer* renderer_;
};