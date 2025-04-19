#pragma once

#include "InspectorPanel.h"
#include "ConsolePanel.h"
#include "HierarchyPanel.h"
#include "ScenePanel.hpp"

#include "WindowManager.h"

/// @addtogroup UI
/// @{
/**
 * @brief Central manager for all ImGui panels in the editor.
 *
 * Handles rendering, initialization, and saving/loading layout settings.
 */
class EditorUIManager {
public:
	/**
	 * @brief Render all the ImGui panels (Scene, Hierarchy, Inspector, Console).
	 */
	void RenderPanels();

	/**
	 * @brief Render the entire ImGui context.
	 */
	void RenderImGui();

	/**
	 * @brief Initializes a new ImGui frame.
	 */
	void InitFrame();

	/**
	 * @brief Sets up ImGui context, styles, and SDL bindings.
	 */
	void InitImGui();

	/**
	 * @brief Save ImGui settings (layout, docking) to a file.
	 *
	 * @param file_path The file to save settings to.
	 */
	void SaveSettings(const std::string& file_path);

	/**
	 * @brief Load ImGui layout settings from file.
	 *
	 * @param file_path The file to load settings from.
	 */
	void LoadSettings(const std::string& file_path);

private:
	HierarchyPanel hierarchy_panel_;	///< Panel showing scene hierarchy
	ConsolePanel console_panel_;		///< Output console panel
	InspectorPanel inspector_panel_;	///< Panel displaying object properties
	ScenePanel scene_panel_;			///< Main viewport panel (game rendering)
};

/// @}