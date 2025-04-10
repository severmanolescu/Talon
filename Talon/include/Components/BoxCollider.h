#pragma once

#include <SDL2/SDL.h>

#include "MindCore.h"
#include "Transform.h"

#include "memory"

class BoxCollider : public MindCore {
public:
	int width_ = 50;
	int height_ = 50;

	SDL_Rect GetBounds() const;

	void Awake() override;

	void DrawDebug(SDL_Renderer* renderer);

	bool draw_debug_ = false;

private:
	std::shared_ptr<Transform> transform_;
};
