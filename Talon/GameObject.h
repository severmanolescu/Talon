#pragma once

#include "MindCore.h"
#include "Transform.h"

#include "string"
#include "memory"
#include "vector"

class GameObject {
public:
	GameObject(const std::string& name) : name_(name) {
		auto transform = std::make_shared<Transform>();
		AddComponent(transform);
	}

	void AddComponent(const std::shared_ptr<MindCore> component) {
		component->game_object_ = this;
		components_.push_back(component);
	}

	std::vector<std::shared_ptr<MindCore>> GetAllComponents() {
		return components_;
	}

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

	std::shared_ptr<Transform> GetTransform() {
		return GetComponent<Transform>();
	}

	void Awake() {
		for (auto& component : components_) {
			component->Awake();
		}
	}

	void Start() {
		for (auto& component : components_) {
			component->Start();
		}
	}

	void Update() {
		for (auto& component : components_) {
			component->Update();
		}
	}

	void OnDestroy() {
		for (auto& component : components_) {
			component->OnDestroy();
		}
	}

	std::string name_;

private:
	std::vector<std::shared_ptr<MindCore>> components_;
};