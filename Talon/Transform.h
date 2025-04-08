#pragma once

#include "MindCore.h"

struct Vector2 {
	float x = .0f;
	float y = .0f;
};

class Transform : public MindCore {
public:
	Vector2 position_;
	Vector2 scale_ = { 1.0f, 1.0f };

	float rotation_ = .0f;
};