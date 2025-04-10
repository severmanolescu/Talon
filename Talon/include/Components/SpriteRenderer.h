#pragma once

#include <SDL2/SDL.h>

#include "MindCore.h"
#include "Transform.h"

#include "memory"

class SpriteRendered : public MindCore {
public:
	int width_ = 50;
	int height_ = 50;
	Uint8 color_r_ = 255, color_g_ = 255, color_b_ = 0;

	SpriteRendered(SDL_Renderer* renderer) : renderer_(renderer) {}

	void Awake() override;

	void Update() override;
private:
	std::shared_ptr<Transform> transform_;

	SDL_Renderer* renderer_;
};
