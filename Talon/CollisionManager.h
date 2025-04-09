#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"
#include "BoxCollider.h"
#include "PhysicsUtils.h" // where CheckCollision() lives

class CollisionManager {
public:
	static bool CheckCollision(GameObject* source, const SDL_Rect& predictedRect) {
		for (auto& obj : scene_) {
			if (obj.get() == source) continue;

			auto collider = obj->GetComponent<BoxCollider>();
			if (!collider) continue;

			if (::CheckCollision(predictedRect, collider->GetBounds())) {
				return true;
			}
		}
		return false;
	}

	static void SetScene(const std::vector<std::shared_ptr<GameObject>>& scene) {
		scene_ = scene;
	}

	static inline std::vector<std::shared_ptr<GameObject>> GetScene() {
		return scene_;
	}

private:
	static inline std::vector<std::shared_ptr<GameObject>> scene_;
};
