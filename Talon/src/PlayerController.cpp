#include "PlayerController.h"
#include "InputSystem.h"
#include "GameObject.h"

#include "iostream"

void PlayerController::Awake() {
	rigidbody_ = game_object_->GetComponent<Rigidbody>();
	animator_ = game_object_->GetComponent<Animator>();

	last_state_ = std::make_pair(animation_state_, direction_);

	spritesheet_layout_[AnimationState::Idle] = { 6, 1 };
	spritesheet_layout_[AnimationState::Walk] = { 4, 1 };

	animation_paths_[{AnimationState::Idle, Direction::Up}] = idle_up_;
	animation_paths_[{AnimationState::Idle, Direction::Down}] = idle_down_;
	animation_paths_[{AnimationState::Idle, Direction::Left}] = idle_left_;
	animation_paths_[{AnimationState::Idle, Direction::Right}] = idle_right_;

	animation_paths_[{AnimationState::Walk, Direction::Up}] = walk_up_;
	animation_paths_[{AnimationState::Walk, Direction::Down}] = walk_down_;
	animation_paths_[{AnimationState::Walk, Direction::Left}] = walk_left_;
	animation_paths_[{AnimationState::Walk, Direction::Right}] = walk_right_;
}

void PlayerController::Start(){
	animator_->SetSpriteSheet(animation_paths_[last_state_], renderer_);

	animator_->sprite_width_ = 16;
	animator_->sprite_height_ = 32;

	animation_state_ = AnimationState::Idle;
	direction_ = Direction::Down;
}

void PlayerController::SetAnimation() {
	animator_->columns_ = spritesheet_layout_[animation_state_].first;
	animator_->rows_ = spritesheet_layout_[animation_state_].second;

	auto key = std::make_pair(animation_state_, direction_);

	if (last_state_ != key)
	{
		last_state_ = key;

		if (animation_paths_.find(key) != animation_paths_.end()) {
			animator_->SetSpriteSheet(animation_paths_[key], renderer_);
		}
		else {
			animator_->SetSpriteSheet(idle_down_, renderer_);
		}
	}
}

void PlayerController::Update() {
	if (!rigidbody_ || !animator_ || !renderer_) {
		return;
	}

	Vector2 movement = { 0, 0 };

	if (InputSystem::GetKey(SDL_SCANCODE_W)) {
		movement.y -= 1.0f;

		direction_ = Direction::Up;
	}
	if (InputSystem::GetKey(SDL_SCANCODE_S)){
		movement.y += 1.0f;
		
		direction_ = Direction::Down;
	}
	if (InputSystem::GetKey(SDL_SCANCODE_A)) { 
		movement.x -= 1.0f; 

		direction_ = Direction::Left;
	}
	if (InputSystem::GetKey(SDL_SCANCODE_D)) {
		movement.x += 1.0f;

		direction_ = Direction::Right;
	}

	if (InputSystem::GetKey(SDL_SCANCODE_LSHIFT)) {
		movement *= sprint_speed_multiplier_;
	}

	if (movement == Vector2::Zero()){
		animation_state_ = AnimationState::Idle;
	}
	else {
		animation_state_ = AnimationState::Walk;
	}

	SetAnimation();

	movement.Normalize();

	rigidbody_->AddForce(movement * .5f);

	if (InputSystem::GetKey(SDL_SCANCODE_SPACE) && rigidbody_->use_gravity_) {
		rigidbody_->AddForce(jump_power_);
	}

}

void PlayerController::OnDestroy() {
}
