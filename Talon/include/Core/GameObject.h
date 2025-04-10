#pragma once

#include "MindCore.h"
#include "Transform.h"

#include "string"
#include "memory"
#include "vector"

class GameObject {
public:
	std::string name_;

	GameObject(const std::string& name) : name_(name) {
		auto transform = std::make_shared<Transform>();
		AddComponent(transform);
	}

	void AddComponent(const std::shared_ptr<MindCore> component);

	std::vector<std::shared_ptr<MindCore>> GetAllComponents();

	template<typename T>
	std::shared_ptr<T> GetComponent() {
		for (auto& component : components_) {
			std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(component);

			if (cast) {
				return cast;
			}
		}

		return nullptr;
	}

	std::shared_ptr<Transform> GetTransform();

	void Awake();

	void Start();

	void Update();

	void OnDestroy();
private:
	std::vector<std::shared_ptr<MindCore>> components_;
};
