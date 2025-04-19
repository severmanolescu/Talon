#include "ScenePanel.hpp"

#include "WindowManager.h"

#include "imgui.h"

void ScenePanel::Render(){
	ImGui::Begin("ScenePanel");

	ImVec2 available_size = ImGui::GetContentRegionAvail();

	if ((int)available_size.x != WindowManager::width_ || (int)available_size.y != WindowManager::height_) {
		WindowManager::ResizeSceneTexture((int)available_size.x, (int)available_size.y);

		WindowManager::width_ = (int)available_size.x;
		WindowManager::height_ = (int)available_size.y;
	}

	ImGui::Image((ImTextureID)WindowManager::GetSceneTexture(), available_size);

	ImGui::End();
}
