#include "AnimatorStateMachine.h"
#include "PlayerController.h"
#include "InputSystem.h"
#include "GameObject.h"

#include "iostream"

void PlayerController::Awake() {
	ui_frame_height_ = 8.0f;

	rigidbody_ = game_object_->GetComponent<Rigidbody>();

	last_state_ = std::make_pair(animation_state_, direction_);

	animator_state_machine_ = game_object_->GetComponent<AnimatorStateMachine>();
}

void PlayerController::Start(){
	animation_state_ = AnimationState::Idle;
	direction_ = Direction::Down;

	animator_state_machine_->SetState("Idle");
	animator_state_machine_->SetInt("direction", 1);
}

void PlayerController::SetAnimatorVariables() {
	if (animation_state_ == AnimationState::Idle) {
		animator_state_machine_->SetBool("isWalking", false);
	}
	else {
		animator_state_machine_->SetBool("isWalking", true);
	}

	switch(direction_) {
		case Direction::Up:
			animator_state_machine_->SetInt("direction", 1);
			break;
		case Direction::Down:
			animator_state_machine_->SetInt("direction", 2);
			break;
		case Direction::Left:
			animator_state_machine_->SetInt("direction", 3);
			break;
		case Direction::Right:
			animator_state_machine_->SetInt("direction", 4);
			break;
	}
}

void PlayerController::Update() {
	if (!rigidbody_ || !renderer_) {
		return;
	}

	Vector2 movement = { 0, 0 };

	if (InputSystem::GetKey("MoveUp")) {
		movement.y -= 1.0f;

		direction_ = Direction::Up;
	}
	if (InputSystem::GetKey("MoveDown")) {
		movement.y += 1.0f;
		
		direction_ = Direction::Down;
	}
	if (InputSystem::GetKey("MoveLeft")) {
		movement.x -= 1.0f; 

		direction_ = Direction::Left;
	}
	if (InputSystem::GetKey("MoveRight")) {
		movement.x += 1.0f;

		direction_ = Direction::Right;
	}

	if (InputSystem::GetKey("Sprint")) {
		movement *= sprint_speed_multiplier_;
	}

	if (movement == Vector2::Zero()){
		animation_state_ = AnimationState::Idle;
	}
	else {
		animation_state_ = AnimationState::Walk;
	}

	SetAnimatorVariables();

	if (movement.x != 0.0f || movement.y != 0.0f) {
		movement.Normalize();

		movement *= walk_speed_;

		rigidbody_->SetVelocity(movement * 0.5f);
	}


	if (InputSystem::GetKey("Jump") && rigidbody_->use_gravity_) {
		rigidbody_->AddForce(jump_power_);
	}
}

void PlayerController::OnDestroy() {
}

void PlayerController::DrawUI(){
	BeginDraw("PlayerController");

	DrawFloatControl("Walk Speed", &walk_speed_);
	DrawFloatControl("Sprint Speed Multiplier", &sprint_speed_multiplier_);

	DrawVector2Control("Jump Power", jump_power_);

	EndDraw();
}
