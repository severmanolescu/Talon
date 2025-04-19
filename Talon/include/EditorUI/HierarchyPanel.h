#pragma once

#include "GameObject.h"

/// Identifier used for drag-and-drop payload.
constexpr const char* hierarchy_gameobject_identifier = "HIERARCHY_GAMEOBJECT";
/// Default name for newly created GameObjects.
constexpr const char* new_game_object_name = "New GameObject";

/// Height of the drag-and-drop drop zone indicator.
constexpr const int drop_zone_height = 2;

/// @addtogroup UI
/// @{

/**
 * @brief Panel that displays the scene hierarchy.
 *
 * Allows selecting, renaming, reordering, reparenting,
 * and deleting GameObjects in a tree structure.
 */
class HierarchyPanel {
public:
	/**
	 * @brief Renders the hierarchy panel.
	 */
	void Render();

	/**
	 * @brief Returns the currently selected GameObject.
	 *
	 * @return A shared pointer to the selected GameObject, or nullptr if none is selected.
	 */
	std::shared_ptr<GameObject> GetSelectedObject() const;

private:
	// Internal state
	std::shared_ptr<GameObject> renaming_target_ = nullptr;

	std::shared_ptr<GameObject> pending_duplicate_ = nullptr;

	std::shared_ptr<GameObject> pending_reparent_target_ = nullptr;
	std::shared_ptr<GameObject> pending_reparent_object_ = nullptr;

	std::shared_ptr<GameObject> pending_reorder_object_ = nullptr;
	std::shared_ptr<GameObject> reorder_target_ = nullptr;

	std::vector<std::shared_ptr<GameObject>> selected_objects_;
	std::shared_ptr<GameObject> last_clicked_object_ = nullptr;

	std::vector<std::shared_ptr<GameObject>> visible_flat_list_;

	bool reorder_above_ = true;

	bool delete_selected_ = false;

	// Input buffers
	char rename_buffer_[128] = "";
	char search_buffer_[128] = "";

	/**
	 * @brief Generates a unique name for a GameObject under a parent.
	 */
	std::string GenerateUniqueName(const std::string& base_name, const std::shared_ptr<GameObject>& parent);

	/**
	 * @brief Draws a single node (GameObject) in the hierarchy tree.
	 */
	void DrawGameObjectNode(const std::shared_ptr<GameObject>& object);

	/**
	 * @brief Checks if the GameObject name matches the current search filter.
	 */
	bool NameMatchesSearch(const std::string& name, const char* search);

	/**
	 * @brief Flattens the scene tree for multi-select and shift-range support.
	 */
	void BuildFlatList(const std::vector<std::shared_ptr<GameObject>>& objects);

	/**
	 * @brief Handles delete, rename, and multi-select keyboard input.
	 */
	void HandleUserKeyboardInput(const std::shared_ptr<GameObject>& object);

	/**
	 * @brief Draws the context menu when right-clicking a GameObject.
	 */
	void DrawPopup(const std::shared_ptr<GameObject>& object);

	/**
	 * @brief Starts a new ImGui panel frame.
	 */
	void BeginPanel();

	/**
	 * @brief Draws a drop zone for drag-and-drop above a list of objects.
	 */
	void DrawDropZoneAbove(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>>);

	/**
	 * @brief Draws a drop zone below a specific GameObject.
	 */
	void DrawDropZoneBelow(std::shared_ptr<GameObject> object);

	/**
	 * @brief Initiates rename mode for a GameObject.
	 */
	void RenameObject(const std::shared_ptr<GameObject>& object);

	/**
	 * @brief Executes any pending operations (deletion, reordering, etc).
	 */
	void HandlePendingActions();
};
/// @}
