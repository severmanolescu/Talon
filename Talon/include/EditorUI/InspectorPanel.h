#pragma once
#include "MindCore.h"

#include <memory>
#include "map"

class GameObject;

class InspectorPanel {
public:
	void Render(std::shared_ptr<GameObject> selected_object);

private:
	bool set_active_ = true;

	char rename_buffer_[128] = "";

	std::map<std::shared_ptr<MindCore>, bool> component_states_;

	std::shared_ptr<GameObject> last_selected_object_ = nullptr;
};
