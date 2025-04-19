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

void SceneManager::LoadScene(){
	std::ifstream file("test.json");
	if (!file.is_open()) {
		LOG_ERROR("[SceneManager] Failed to open file: test.json");
		return;
	}

	nlohmann::json json_data;
	file >> json_data;

	for (const auto& game_object : json_data["game_objects"]) {
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>(game_object["name"], game_object["uuid"]);
		
		for (const auto& component : game_object["components"]) {
			if (component.contains("data")) {
				if (component["type"] == "Transform") {
					object->GetTransform()->Deserialize(component["data"]);

					if (component.contains("active")) {
						object->GetTransform()->active_ = component["active"];
					}

					continue;
				}
				std::shared_ptr<MindCore> new_component = ComponentFactory::Instance().Create(component["type"]);

				new_component->Deserialize(component["data"]);

				if (component.contains("active")) {
					new_component->active_ = component["active"];
				}

				object->AddComponent(new_component);
			}
		}

		MindEngine::AddGameObject(object);
	}

}
