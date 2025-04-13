#pragma once
#include <memory>

class GameObject;

class InspectorPanel {
public:
	void Render();
	void SetSelectedObject(std::shared_ptr<GameObject> object);

private:
	std::shared_ptr<GameObject> selected_;
};
