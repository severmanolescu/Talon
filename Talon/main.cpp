#include "PlayerController.h"
#include "GameObject.h"
#include "Rigidbody2D.h"

#include <vector>
#include <memory>
#include <iostream>

int main() {
	std::vector<std::shared_ptr<GameObject>> scene;

	auto player = std::make_shared<GameObject>("Player");
	player->AddComponent(std::make_shared<PlayerController>());
	auto rigidbody = std::make_shared<Rigidbody2D>();

	player->AddComponent(rigidbody);

	rigidbody->gravity_ = false;

	rigidbody->AddForce({ 1.0f, -2.0f });

	scene.push_back(player);

	for (auto& object : scene) {
		object->Awake();
	}

	for (auto& object : scene) {
		object->Start();
	}

	bool running = true;

	short frame = 0;

	while (running) {
		for (auto& object : scene) {
			auto obj_transform = object->GetComponent<Transform>();

			object->Update();

			if (obj_transform) {
				std::cout << "Position: " << std::endl;
				std::cout << "x: " << obj_transform->position_.x << std::endl;
				std::cout << "y: " << obj_transform->position_.y << std::endl;
			}
		}

		if (++frame > 100) {
			break;
		}
	}

	for (auto& object : scene) {
		object->OnDestroy();
	}

	return 0;
}