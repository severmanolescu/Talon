#pragma once
#include "MindCore.h"

#include <memory>
#include "map"

class GameObject;

/// @addtogroup UI
/// @{

/**
 * @brief Panel responsible for displaying and editing components of a selected GameObject.
 *
 * Provides functionality to rename the object, toggle component activity,
 * and display serialized UI for each component.
 */
class InspectorPanel {
public:
	/**
	 * @brief Renders the inspector panel for the given GameObject.
	 *
	 * @param selected_object The currently selected GameObject.
	 */
	void Render(std::shared_ptr<GameObject> selected_object);

private:
	/// Determines if the selected object is active.
	bool set_active_ = true;

	/// Input buffer for renaming GameObjects.
	char rename_buffer_[128] = "";

	/// Tracks the active/inactive state of each component.
	std::map<std::shared_ptr<MindCore>, bool> component_states_;

	/// Stores the last selected object to detect changes.
	std::shared_ptr<GameObject> last_selected_object_ = nullptr;
};
/// @}
