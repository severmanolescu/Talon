#include "PlayerController.h"
#include "iostream"

void PlayerController::Awake() {
	std::cout << "PlayerController: Awake \n";
}

void PlayerController::Start(){
	std::cout << "PlayerController: Start \n";
}

void PlayerController::Update() {
	std::cout << "PlayerController: Update \n";
}

void PlayerController::OnDestroy() {
	std::cout << "PlayerController: OnDestroy \n";
}