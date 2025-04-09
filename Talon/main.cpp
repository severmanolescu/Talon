#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "WindowManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "PlayerController.h"
#include "Rigidbody2D.h"
#include "SpriteRenderer.h"
#include "InputSystem.h"
#include "BoxCollider.h"
#include "CollisionManager.h"

#include <vector>
#include <memory>
#include <iostream>


int main() {
	WindowManager window;
	if (!window.Init("Talon Engine", 800, 600)) {
		return -1;
	}

	std::vector<std::shared_ptr<GameObject>> scene;

	auto player = std::make_shared<GameObject>("Player");
	player->AddComponent(std::make_shared<PlayerController>());
	player->AddComponent(std::make_shared<BoxCollider>());
	auto rigidbody = std::make_shared<Rigidbody2D>();

	player->AddComponent(rigidbody);

	rigidbody->use_gravity_ = false;

	player->AddComponent(std::make_shared<SpriteRendered>(window.GetRenderer()));

	player->GetTransform()->position_ = { 100.0f, 150.0f };

	auto wall = std::make_shared<GameObject>("Wall");

	std::shared_ptr<Transform> wall_transform = wall->GetTransform();

	wall_transform->position_ = {100.0f, 200.0f};
	wall_transform->scale_ = { 100.0f, 1.0f };

	std::shared_ptr<BoxCollider> box_collider = std::make_shared<BoxCollider>();
	std::shared_ptr<SpriteRendered> sprite_rendered = std::make_shared<SpriteRendered>(window.GetRenderer());

	box_collider->width_ = 100;
	box_collider->height_ = 50;

	sprite_rendered->width_ = 100;
	sprite_rendered->height_ = 50;

	wall->AddComponent(box_collider);

	wall->AddComponent(sprite_rendered);

	scene.push_back(player);
	scene.push_back(wall);

	CollisionManager::SetScene(scene);

	for (auto& object : scene) {
		object->Awake();
	}

	for (auto& object : scene) {
		object->Start();
	}

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) running = false;
		}

		window.Clear();

		InputSystem::Update();

		for (auto& object : scene) {
			object->Update();
		}

		InputSystem::LateUpdate();

		window.Present();
		SDL_Delay(16);
	}

	for (auto& object : scene) {
		object->OnDestroy();
	}

	window.Shutdown();

	return 0;
}