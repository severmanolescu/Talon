#pragma once

#include "MindCore.h"
#include "Vector2.h"

class Transform : public MindCore {
public:
	Vector2 position_;
	Vector2 last_position_;

	Vector2 scale_ = { 1.0f, 1.0f };

	float rotation_ = .0f;

	void Update() override {
		last_position_ = position_;
	}
};