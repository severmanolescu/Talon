#pragma once

#include "GameObject.h"

class MindEngine {
public:
	static std::shared_ptr<GameObject> FindGameObject(const std::string& name);

	static std::shared_ptr<GameObject> FindInChildren(const std::shared_ptr<GameObject>& obj, const std::string& name);

	static void RemoveGameObject(const std::shared_ptr<GameObject>& object);

	static void AddGameObject(const std::shared_ptr<GameObject>& game_object);

	static std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GetAllGameObjects();

private:
	static std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> game_objects_;

};