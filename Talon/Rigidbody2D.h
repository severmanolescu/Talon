#pragma once

#include "MindCore.h"
#include "Transform.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "CollisionManager.h"

#include "memory"
#include "iostream"

template <typename T>
T clamp(T value, T min, T max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

class Rigidbody2D : public MindCore {
public:
	Vector2 velocity_ = { .0f, .0f };
	Vector2 acceleration_ = { .0f, .0f };

	Vector2 linear_drag_ = {.9f, .9f};
	
	float mass_ = 1.0f;

	float gravity_ = 9.81f;

	float max_velocity_ = 5.0f;

	bool use_gravity_ = true;

	bool is_kinematic_ = false;

	void Awake() override{
		if (game_object_) {
			transform_ = game_object_->GetComponent<Transform>();

			if (!transform_) {
				std::cerr << "[Rigidbody2D] Error: No transform found on object: " << game_object_->name_;
			}
		}
	}

	void ClampVelocity() {
		velocity_.x = clamp(velocity_.x, -max_velocity_, max_velocity_);
		velocity_.y = clamp(velocity_.y, -max_velocity_, max_velocity_);

	}

	void AddForce(const Vector2& force) {
		acceleration_.x += force.x / mass_;
		acceleration_.y += force.y / mass_;
	}

	void SetVelocity(const Vector2& new_velocity) {
		velocity_ = new_velocity;
	}

	void AddVelocity(const Vector2& delta_velocity) {
		velocity_.x += delta_velocity.x;
		velocity_.y += delta_velocity.y;
	}

	void ApplyImpulse(const Vector2& impulse) {
		velocity_.x += impulse.x / mass_;
		velocity_.y += impulse.y / mass_;
	}

	void OnCollision() {
		velocity_ = { 0.0f, 0.0f };
		acceleration_ = { 0.0f, 0.0f };
	}

	void CheckAndResolveCollision() {
		auto transform = game_object_->GetTransform();
		auto collider = game_object_->GetComponent<BoxCollider>();
		if (!transform || !collider) return;

		int stepsX = static_cast<int>(std::abs(velocity_.x));
		int signX = (velocity_.x > 0) ? 1 : -1;

		for (int i = 0; i < stepsX; ++i) {
			transform->position_.x += signX;

			SDL_Rect testRect = collider->GetBounds();
			if (CollisionManager::CheckCollision(game_object_, testRect)) {
				transform->position_.x -= signX;
				velocity_.x = 0;
				break;
			}
		}

		int stepsY = static_cast<int>(std::abs(velocity_.y));
		int signY = (velocity_.y > 0) ? 1 : -1;

		for (int i = 0; i < stepsY; ++i) {
			transform->position_.y += signY;

			SDL_Rect testRect = collider->GetBounds();
			if (CollisionManager::CheckCollision(game_object_, testRect)) {
				transform->position_.y -= signY;
				velocity_.y = 0;
				break;
			}
		}
	}


	void Update() override {
		if (!transform_ || is_kinematic_) return;

		if (use_gravity_) {
			acceleration_.y += gravity_; 
		}

		velocity_.x += acceleration_.x;
		velocity_.y += acceleration_.y;

		velocity_.x *= linear_drag_.x;
		velocity_.y *= linear_drag_.y;

		if (std::abs(velocity_.x) < 0.01f) velocity_.x = 0.0f;
		if (std::abs(velocity_.y) < 0.01f) velocity_.y = 0.0f;

		ClampVelocity();

		CheckAndResolveCollision();

		acceleration_ = { 0.0f, 0.0f };
	}

private:
	std::shared_ptr<Transform> transform_;
};