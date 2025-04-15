#pragma once

#include "GameObject.h"
#include "BoxCollider.h"
#include "PhysicsUtils.h"
#include "MindEngine.h"

#include <vector>
#include <memory>

class CollisionManager {
public:
	static bool CheckCollision(GameObject* source, const SDL_Rect& predictedRect) {
		std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> scene = MindEngine::GetAllGameObjects();

		for (auto& obj : *scene) {
			if (obj.get() == source) continue;

			auto collider = obj->GetComponent<BoxCollider>();
			if (!collider) continue;

			if (::CheckCollision(predictedRect, collider->GetBounds())) {
				return true;
			}
		}
		return false;
	}
};
