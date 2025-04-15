#include "EditorUIManager.h"
#include "WindowManager.h"

#include <imgui.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <fstream>
#include "json.hpp"

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

	ImGuiID dockspace_id = ImGui::GetID("TalonEngineDock");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	ImGui::End();

    ImGui::DockSpaceOverViewport(ImGui::GetID("TalonDockspace"), ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

    hierarchy_panel_.Render();
	inspector_panel_.Render(hierarchy_panel_.GetSelectedObject());
    console_panel_.Render();
    scene_panel_.Render();
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
