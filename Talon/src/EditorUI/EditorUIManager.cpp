#include "EditorUIManager.h"
#include "WindowManager.h"
#include "SceneManager.h"
#include "MindEngine.h"

#include <imgui.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <fstream>
#include "json.hpp"

void EditorUIManager::RenderTopToolbar(ImGuiViewport* viewport) {
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, toolbar_height_));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 6.0f));

	ImGui::Begin("Toolbar", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoDocking);

	float toolbar_width = ImGui::GetContentRegionAvail().x;
	float button_width = 60.0f;
	ImGui::SetCursorPosX((toolbar_width - button_width) * 0.5f);

	if (SceneManager::IsPlaying()) {
		if (ImGui::Button("Stop")) {
			SceneManager::SetRunningMode(EngineMode::Edit);

			MindEngine::RemoveAllGameObjects();

			SceneManager::LoadScene();
		}
	}
	else {
		if (ImGui::Button("Play")) {
			SceneManager::SaveScene();

			SceneManager::SetRunningMode(EngineMode::Play);
		}
	}

	ImGui::End();
	ImGui::PopStyleVar(3);
}

void EditorUIManager::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(WindowManager::GetWindow(), WindowManager::GetRenderer());
    ImGui_ImplSDLRenderer2_Init(WindowManager::GetRenderer());
}

void EditorUIManager::SaveSettings(const std::string& file_path){
	nlohmann::json data;

	console_panel_.SaveSettings(data);

	std::ofstream file(file_path);
	if (file.is_open())
		file << data.dump(4);
}

void EditorUIManager::LoadSettings(const std::string& file_path){
	std::ifstream file(file_path);
	if (!file.is_open()) return;

	nlohmann::json data;
	file >> data;

	console_panel_.LoadSettings(data);
}

void EditorUIManager::RenderPanels() {
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	RenderTopToolbar(viewport);

	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + toolbar_height_));
	ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - toolbar_height_));
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags dockspace_flags =
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("DockSpaceRegion", nullptr, dockspace_flags);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	scene_panel_.Render();
	game_panel_.Render();
	inspector_panel_.Render(hierarchy_panel_.GetSelectedObject());
	hierarchy_panel_.Render();
	console_panel_.Render();

	ImGui::End();
}

void EditorUIManager::InitFrame(){
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui::NewFrame();
}

void EditorUIManager::RenderImGui(){
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), WindowManager::GetRenderer());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		SDL_Window* backup_window = SDL_GL_GetCurrentWindow();
		SDL_Renderer* backup_renderer = SDL_GetRenderer(backup_window);
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
