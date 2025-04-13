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
#include "InputSystem.h"
#include "EditorUIManager.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <vector>
#include <memory>
#include <iostream>

WindowManager window;

std::vector<std::shared_ptr<GameObject>> scene;
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

	scene.push_back(player);
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

	scene.push_back(wall);
}

int main() {
	if (!window.Init("Talon Engine", 800, 600)) {
		return -1;
	}

	EditorUIManager editor_ui_manager;

	// Init ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Style
	ImGui::StyleColorsDark(); // or Light, Classic

	// Init SDL bindings
	ImGui_ImplSDL2_InitForSDLRenderer(window.GetWindow(), window.GetRenderer());

	ImGui_ImplSDLRenderer2_Init(window.GetRenderer());

	InputSystem::LoadFromJson("./assets/config/input.json");

	SetUpPlayer();

	SetUpWall();

	CollisionManager::SetScene(scene);

	for (auto& object : scene) {
		object->Awake();
	}

	for (auto& object : scene) {
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

		ImGui_ImplSDL2_NewFrame();
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui::NewFrame();

		// Fullscreen DockSpace host window
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Root", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		ImGui::End();


		window.Clear();

		InputSystem::Update();

		editor_ui_manager.Render();

		for (auto& object : scene) {
			object->Update();

			auto collider = object->GetComponent<BoxCollider>();

			if (collider) {
				collider->DrawDebug(window.GetRenderer());
			}
		}

		InputSystem::LateUpdate();

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), window.GetRenderer());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			SDL_Window* backup_window = SDL_GL_GetCurrentWindow();
			SDL_Renderer* backup_renderer = SDL_GetRenderer(backup_window);
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		window.Present();
		SDL_Delay(16);
	}

	for (auto& object : scene) {
		object->OnDestroy();
	}

	window.Shutdown();

	return 0;
}
