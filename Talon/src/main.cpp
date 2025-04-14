#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include "WindowManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "PlayerController.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "InputSystem.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "Animator.h"
#include "AnimatorStateMachine.h"
#include "EditorUIManager.h"
#include "Console.hpp"
#include "MindEngine.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <vector>
#include <memory>
#include <iostream>

WindowManager window;

std::shared_ptr<GameObject> player;

void SetUpPlayer() {
	player = std::make_shared<GameObject>("Player");
	
	std::shared_ptr<PlayerController> player_controller = std::make_shared<PlayerController>();

	player_controller->renderer_ = window.GetRenderer();

	player_controller->idle_up_ = "./assets/player/player_idle_up.png";
	player_controller->idle_down_ = "./assets/player/player_idle_down.png";
	player_controller->idle_left_ = "./assets/player/player_idle_left.png";
	player_controller->idle_right_ = "./assets/player/player_idle_right.png";

	player_controller->walk_up_ = "./assets/player/player_walk_up.png";
	player_controller->walk_down_ = "./assets/player/player_walk_down.png";
	player_controller->walk_left_ = "./assets/player/player_walk_left.png";
	player_controller->walk_right_ = "./assets/player/player_walk_right.png";

	player->AddComponent(player_controller);

	// Player BoxCollider
	std::shared_ptr<BoxCollider> box_collider_player = std::make_shared<BoxCollider>();

	box_collider_player->width_ = 25;
	box_collider_player->height_ = 20;
	box_collider_player->offset_ = { 4, 40 };
	box_collider_player->draw_debug_ = true;

	player->AddComponent(box_collider_player);

	// Player Rigidbody
	auto rigidbody = std::make_shared<Rigidbody>();

	rigidbody->use_gravity_ = false;

	player->AddComponent(rigidbody);

	// Player AnimatorStateMachine
	std::shared_ptr<AnimatorStateMachine> animator_state_machine = std::make_shared<AnimatorStateMachine>();

	animator_state_machine->LoadFromJson("./assets/config/player/animation_state.json");

	player->AddComponent(animator_state_machine);

	// Player SpriteRendered
	std::shared_ptr<SpriteRenderer> sprite_rendered_player = std::make_shared<SpriteRenderer>(window.GetRenderer());

	//sprite_rendered_player->SetImage("./assets/player/player_down.png");

	player->AddComponent(sprite_rendered_player);

	// Set Player position
	player->GetTransform()->position_ = { 100.0f, 150.0f };

	// Set Animator
	std::shared_ptr<Animator> animator = std::make_shared<Animator>();

	player->AddComponent(animator);

	player->AddChild(std::make_shared<GameObject>("Weapon"));
	player->AddChild(std::make_shared<GameObject>("Backack"));
	player->AddChild(std::make_shared<GameObject>("Bow"));
	player->AddChild(std::make_shared<GameObject>("Food"));

	MindEngine::AddGameObject(player);
}

void SetUpWall() {
	auto wall = std::make_shared<GameObject>("Wall");

	std::shared_ptr<Transform> wall_transform = wall->GetTransform();

	wall_transform->position_ = { 100.0f, 400.0f };
	wall_transform->scale_ = { 32, 32 };

	std::shared_ptr<BoxCollider> box_collider = std::make_shared<BoxCollider>();
	std::shared_ptr<SpriteRenderer> sprite_rendered_wall = std::make_shared<SpriteRenderer>(window.GetRenderer());

	sprite_rendered_wall->SetImage("assets\\player\\grass.png");

	box_collider->width_ = 32;
	box_collider->height_ = 32;
	box_collider->draw_debug_ = true;

	sprite_rendered_wall->width_ = 32;
	sprite_rendered_wall->height_ = 32;

	wall->AddComponent(box_collider);

	wall->AddComponent(sprite_rendered_wall);

	MindEngine::AddGameObject(wall);
}

int main() {
	if (!window.Init("Talon Engine", 800, 600)) {
		return -1;
	}

	EditorUIManager editor_ui_manager;

	editor_ui_manager.InitImGui(window.GetWindow(), window.GetRenderer());

	InputSystem::LoadFromJson("./assets/config/input.json");
	editor_ui_manager.LoadSettings("./settings/editor_settings.json");

	SetUpPlayer();

	SetUpWall();

	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> scene = MindEngine::GetAllGameObjects();

	CollisionManager::SetScene(*scene);

	LOG_ERROR("Talon Engine started!");
	LOG_INFO("Talon Engine started!");

	if (scene == nullptr) {
		LOG_ERROR("Scene is null");
		return -1;
	}

	for (auto& object : *scene) {
		object->Awake();
	}

	for (auto& object : *scene) {
		object->Start();
	}

	player->GetComponent<Animator>()->SetRender(window.GetRenderer());

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) running = false;
		}

		editor_ui_manager.InitFrame();

		SDL_SetRenderTarget(window.GetRenderer(), window.GetSceneTexture());
		SDL_SetRenderDrawColor(window.GetRenderer(), 0, 0, 0, 255);
		SDL_RenderClear(window.GetRenderer());

		for (auto& object : *scene) {
			object->Update();

			if (auto collider = object->GetComponent<BoxCollider>()) {
				collider->DrawDebug(window.GetRenderer());
			}
		}

		SDL_SetRenderTarget(window.GetRenderer(), nullptr);

		window.Clear();

		// --- RENDER IMGUI ---
		InputSystem::Update();

		editor_ui_manager.RenderPanels();

		ImGui::Begin("Scene");
		ImGui::Image((ImTextureID)window.GetSceneTexture(), ImVec2(800, 600));
		ImGui::End();

		InputSystem::LateUpdate();
		editor_ui_manager.RenderImGui(window.GetRenderer());
		window.Present();

		SDL_Delay(16);
	}


	for (auto& object : *scene) {
		object->OnDestroy();
	}

	window.Shutdown();

	editor_ui_manager.SaveSettings("./settings/editor_settings.json");

	return 0;
}
