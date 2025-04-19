#pragma once

#include "GameObject.h"
#include "BoxCollider.h"
#include "PhysicsUtils.h"
#include "MindEngine.h"

#include <vector>
#include <memory>

/**
 * @brief Static class responsible for handling collision checks between GameObjects.
 */
class CollisionManager {
public:
	/**
	 * @brief Checks whether the given predicted rectangle collides with any other collider in the scene.
	 *
	 * @param source The GameObject initiating the collision check (ignored in the comparison).
	 * @param predictedRect The rectangle representing the future position/size of the source.
	 * @return true If a collision is detected with any other GameObject.
	 * @return false If no collision is detected.
	 */
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
