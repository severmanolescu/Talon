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
#include "SceneManager.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "ComponentFactory.h"

#include <vector>
#include <memory>
#include <iostream>

EngineMode SceneManager::running_mode_ = EngineMode::Edit;

std::shared_ptr<GameObject> player;

void SetUpPlayer() {
	player = std::make_shared<GameObject>("Player");

	std::shared_ptr<PlayerController> player_controller = std::make_shared<PlayerController>();

	player_controller->renderer_ = WindowManager::GetRenderer();

	player->AddComponent(player_controller);

	// Player BoxCollider
	std::shared_ptr<BoxCollider> box_collider_player = std::make_shared<BoxCollider>();

	box_collider_player->width_ = 25;
	box_collider_player->height_ = 20;
	box_collider_player->offset_ = { 4, 40 };
	box_collider_player->draw_debug_ = true;

	player->AddComponent(box_collider_player);

	// Player SpriteRendered
	std::shared_ptr<SpriteRenderer> sprite_rendered_player = std::make_shared<SpriteRenderer>(WindowManager::GetRenderer());

	sprite_rendered_player->SetImage("./assets/player/player_down.png");

	sprite_rendered_player->width_ = 32;
	sprite_rendered_player->height_ = 64;

	player->AddComponent(sprite_rendered_player);

	// Player Rigidbody
	auto rigidbody = std::make_shared<Rigidbody>();

	rigidbody->use_gravity_ = false;

	player->AddComponent(rigidbody);

	// Player AnimatorStateMachine
	std::shared_ptr<AnimatorStateMachine> animator_state_machine = std::make_shared<AnimatorStateMachine>();

	animator_state_machine->LoadFromJson("./assets/config/player/animation_state.json");

	player->AddComponent(animator_state_machine);

	// Set Player position
	player->GetTransform()->position_ = { 100.0f, 150.0f };

	// Set Animator
	std::shared_ptr<Animator> animator = std::make_shared<Animator>();

	player->AddComponent(animator);

	std::shared_ptr<GameObject> weapon = std::make_shared<GameObject>("Weapon");

	weapon->AddComponent(std::make_shared<SpriteRenderer>(WindowManager::GetRenderer()));

	weapon->GetComponent<SpriteRenderer>()->width_ = 16;
	weapon->GetComponent<SpriteRenderer>()->height_ = 16;

	weapon->GetComponent<SpriteRenderer>()->SetImage("./assets/player/weapon.png");

	weapon->AddComponent(std::make_shared<BoxCollider>());

	player->AddChild(weapon);
	player->AddChild(std::make_shared<GameObject>("Backpack"));
	player->AddChild(std::make_shared<GameObject>("Bow"));
	player->AddChild(std::make_shared<GameObject>("Food"));

	MindEngine::AddGameObject(player);
}

void SetUpWall() {
	auto wall = std::make_shared<GameObject>("Wall");

	std::shared_ptr<Transform> wall_transform = wall->GetTransform();

	wall_transform->position_ = { 100.0f, 400.0f };
	wall_transform->scale_ = { 1, 1 };

	std::shared_ptr<BoxCollider> box_collider = std::make_shared<BoxCollider>();
	std::shared_ptr<SpriteRenderer> sprite_rendered_wall = std::make_shared<SpriteRenderer>(WindowManager::GetRenderer());

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

void Play() {
	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> scene = MindEngine::GetAllGameObjects();

	for (auto& object : *scene) {
		object->Awake();
	}

	for (auto& object : *scene) {
		object->Start();
	}

	Console::Info("Play mode started", __FILE__, __LINE__);
}

int main() {
	if (!WindowManager::Init("Talon Engine", 800, 600)) {
		return -1;
	}

	ComponentFactory::Instance().Register("Animator", &Animator::Create);
	ComponentFactory::Instance().Register("BoxCollider", &BoxCollider::Create);
	ComponentFactory::Instance().Register("PlayerController", &PlayerController::Create);
	ComponentFactory::Instance().Register("Rigidbody", &Rigidbody::Create);
	ComponentFactory::Instance().Register("SpriteRenderer", &SpriteRenderer::Create);
	ComponentFactory::Instance().Register("Transform", &Transform::Create);
	ComponentFactory::Instance().Register("AnimatorStateMachine", &AnimatorStateMachine::Create);

	EditorUIManager editor_ui_manager;

	editor_ui_manager.InitImGui();

	InputSystem::LoadFromJson("./assets/config/input.json");
	editor_ui_manager.LoadSettings("./settings/editor_settings.json");

	//SetUpPlayer();

	//SetUpWall();

	SceneManager::LoadScene();

	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> scene = MindEngine::GetAllGameObjects();

	if (scene == nullptr) {
		LOG_ERROR("Scene is null");
		return -1;
	}

	//player->GetComponent<Animator>()->SetRender(WindowManager::GetRenderer());

	bool running = true;
	bool called_play = false;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) running = false;
		}

		if (SceneManager::IsPlaying() && !called_play) {
			Play();
			called_play = true;
		}
		else if (!SceneManager::IsPlaying()) {
			called_play = false;
		}

		editor_ui_manager.InitFrame();

		SDL_SetRenderTarget(WindowManager::GetRenderer(), WindowManager::GetSceneTexture());
		SDL_SetRenderDrawColor(WindowManager::GetRenderer(), 0, 0, 0, 255);
		SDL_RenderClear(WindowManager::GetRenderer());

		if (SceneManager::IsPlaying()) {
			for (auto& object : *scene) {
				object->Update();
			}
		}
		else {
			for (auto& object : *scene) {
				object->Render();
			}
		}

		for (auto& object : *scene) {
			object->DrawGizmo();
		}

		SDL_SetRenderTarget(WindowManager::GetRenderer(), nullptr);

		WindowManager::Clear();

		InputSystem::Update();

		editor_ui_manager.RenderPanels();
		editor_ui_manager.RenderImGui();

		InputSystem::LateUpdate();

		WindowManager::Present();

		SDL_Delay(16);
	}

	for (auto& object : *scene) {
		object->OnDestroy();
	}

	WindowManager::Shutdown();

	//SceneManager::SaveScene();

	editor_ui_manager.SaveSettings("./settings/editor_settings.json");

	return 0;
}
