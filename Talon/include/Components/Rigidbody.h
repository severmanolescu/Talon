#pragma once

#include "MindCore.h"
#include "Transform.h"

#include "memory"

template <typename T>
T clamp(T value, T min, T max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

class Rigidbody : public MindCore {
public:
	Vector2 velocity_ = { .0f, .0f };
	Vector2 acceleration_ = { .0f, .0f };

	Vector2 linear_drag_ = {.9f, .9f};
	
	float mass_ = 1.0f;

	float gravity_ = 9.81f;

	float max_velocity_ = 5.0f;

	bool use_gravity_ = true;

	bool is_kinematic_ = false;

	void Awake() override;

	void ClampVelocity();

	void AddForce(const Vector2& force);

	void SetVelocity(const Vector2& new_velocity);

	void AddVelocity(const Vector2& delta_velocity);

	void ApplyImpulse(const Vector2& impulse);

	void OnCollision();

	void CheckAndResolveCollision();

	void Update() override;

private:
	std::shared_ptr<Transform> transform_;
};
