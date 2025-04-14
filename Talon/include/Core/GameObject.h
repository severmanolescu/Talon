#pragma once

#include "Transform.h"

#include "string"
#include "memory"
#include "vector"

class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	std::string name_;

	std::vector<std::shared_ptr<GameObject>> childrens_;

	std::weak_ptr<GameObject> parent_;

	GameObject(const std::string& name) : name_(name) {
		auto transform = std::make_shared<Transform>();
		AddComponent(transform);
	}

	void AddComponent(const std::shared_ptr<MindCore> component);

	std::vector<std::shared_ptr<MindCore>> GetAllComponents();

	std::shared_ptr<Transform> GetTransform();

	void Awake();

	void Start();

	void Update();

	void OnDestroy();

	void AddChild(std::shared_ptr<GameObject> child);

	bool HasParent();

	void DrawGizmo();

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

	template<typename T>
	std::shared_ptr<T> GetComponentInChilds() {
		for (auto& child : childrens_)
		{
			for (auto& component : child->components_) {
				std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(component);

				if (cast) {
					return cast;
				}
			}

			auto nested = child->GetComponentInChilds<T>();
			if (nested) return nested;
		}

		return nullptr;
	}

	template<typename T>
	std::shared_ptr<T> GetComponentsInChilds() {
		std::shared_ptr<std::vector<std::shared_ptr<T>>> components = std::make_shared<std::vector<std::shared_ptr<T>>>();

		for (auto& child : childrens_)
		{
			for (auto& component : child->components_) {
				std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(component);

				if (cast) {
					components->push_back(cast);
				}
			}

			auto childComponents = child->GetComponentsInChilds<T>();
			components->insert(components->end(), childComponents->begin(), childComponents->end());
		}

		return components;
	}

	template<typename T>
	std::shared_ptr<T> GetComponentInParent() {
		if (HasParent())
		{
			for (auto& component : parent_->components_) {
				std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(component);

				if (cast) {
					return cast;
				}
			}
		}

		return nullptr;
	}

	bool IsActive() {
		return active_;
	}

	void SetActive(bool active) {
		active_ = active;
	}

	void RemoveComponent(const std::shared_ptr<MindCore> component);
private:
	std::vector<std::shared_ptr<MindCore>> components_;
	std::vector<std::shared_ptr<MindCore>> to_remove_components_;

	bool active_ = true;
};
