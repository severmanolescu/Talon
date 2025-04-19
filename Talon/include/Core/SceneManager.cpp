#include "SceneManager.h"
#include "MindEngine.h"
#include "Console.hpp"
#include "ComponentFactory.h"

#include "PlayerController.h"

#include "json.hpp"

#include <fstream>

void SceneManager::SaveScene(){
	nlohmann::json data;

	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> objects = MindEngine::GetAllGameObjects();

	data["game_objects"] = nlohmann::json::array();

	for (auto& object : *objects) {
		object->Serialize(data["game_objects"]);
	}

	std::ofstream file("test.json");
	if (file.is_open())
		file << data.dump(4);
}

void SceneManager::LoadScene() {
	std::ifstream file("test.json");
	if (!file.is_open()) {
		LOG_ERROR("[SceneManager] Failed to open file: test.json");
		return;
	}

	nlohmann::json json_data;
	file >> json_data;

	std::unordered_map<std::string, std::shared_ptr<GameObject>> object_map;

	for (const auto& game_object : json_data["game_objects"]) {
		std::string name = game_object["name"];
		std::string uuid = game_object["uuid"];

		auto object = std::make_shared<GameObject>(name, uuid);

		for (const auto& component : game_object["components"]) {
			if (component.contains("data")) {
				if (component["type"] == "Transform") {
					object->GetTransform()->Deserialize(component["data"]);
					if (component.contains("active")) object->GetTransform()->active_ = component["active"];
					continue;
				}
				auto new_component = ComponentFactory::Instance().Create(component["type"]);
				if (!new_component) continue;

				new_component->Deserialize(component["data"]);
				if (component.contains("active")) new_component->active_ = component["active"];

				object->AddComponent(new_component);
			}
		}

		object_map[uuid] = object;
	}

	for (const auto& game_object : json_data["game_objects"]) {
		std::string uuid = game_object["uuid"];
		auto object = object_map[uuid];

		if (game_object.contains("parent") && !game_object["parent"].is_null()) {
			std::string parent_uuid = game_object["parent"];
			auto parent = object_map[parent_uuid];
			parent->AddChild(object);
		}
	}

	for (const auto& [uuid, object] : object_map) {
		if (!object->HasParent()) {
			MindEngine::AddGameObject(object);
		}
	}
}
