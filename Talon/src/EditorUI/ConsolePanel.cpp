#include "ConsolePanel.h"

#include "imgui.h"

void ConsolePanel::Render(){
	ImGui::Begin("Console");
	ImGui::Text("Ready.");
	ImGui::End();
}
