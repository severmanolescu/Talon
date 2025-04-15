#pragma once

#include <SDL2/SDL.h>

#include "MindCore.h"
#include "Transform.h"

#include "memory"

class BoxCollider : public MindCore {
public:
	BoxCollider() {
		priority = 5;
	}

	int width_ = 50;
	int height_ = 50;

	Vector2 offset_ = { .0f, .0f };

	bool draw_debug_ = false;

	SDL_Rect GetBounds() const;

	void Awake() override;

	void DrawGizmo() override;

	void DrawUI() override;

private:
	std::shared_ptr<Transform> transform_;
};
