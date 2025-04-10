#include "PlayerController.h"
#include "InputSystem.h"
#include "GameObject.h"

void PlayerController::Awake() {
	rigidbody_ = game_object_->GetComponent<Rigidbody>();
	box_collider_ = game_object_->GetComponent<BoxCollider>();
}

void PlayerController::Start(){
}

void PlayerController::Update() {
	if (!rigidbody_ || !box_collider_) {
		return;
	}

	Vector2 movement = { 0, 0 };

	if (InputSystem::GetKey(SDL_SCANCODE_W)) movement.y -= 1.0f;
	if (InputSystem::GetKey(SDL_SCANCODE_S)) movement.y += 1.0f;
	if (InputSystem::GetKey(SDL_SCANCODE_A)) movement.x -= 1.0f;
	if (InputSystem::GetKey(SDL_SCANCODE_D)) movement.x += 1.0f;

	rigidbody_->AddForce(movement * 2.0f);

	if (InputSystem::GetKey(SDL_SCANCODE_SPACE) && rigidbody_->use_gravity_) {
		rigidbody_->AddForce(jump_power_);
	}

}

void PlayerController::OnDestroy() {
}
