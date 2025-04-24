#pragma once

#include "Transform.h"

#include "string"
#include "memory"
#include "vector"

/**
 * @class GameObject
 * @brief Core container class for components in the scene.
 *
 * Represents an entity in the scene that can contain components like Transform,
 * Rigidbody, SpriteRenderer, etc. Supports a hierarchy system with children and a parent.
 */
class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	/// Name of the GameObject, shown in the editor.
	std::string name_;

	/// List of child GameObjects (hierarchical structure).
	std::vector<std::shared_ptr<GameObject>> childrens_;

	/// Weak reference to the parent GameObject.
	std::weak_ptr<GameObject> parent_;

	/// Whether this GameObject is active in the scene.
	bool active_ = true;

	/**
	 * @brief Constructs a new GameObject with a name.
	 * Automatically adds a Transform component.
	 */
	GameObject(const std::string& name) : name_(name) {
		auto transform = std::make_shared<Transform>();
		AddComponent(transform);
		GenerateUUID();
	}

	/**
	 * @brief Adds a component to the GameObject.
	 */
	void AddComponent(const std::shared_ptr<MindCore> component);

	/**
	 * @brief Returns all attached components.
	 */
	std::vector<std::shared_ptr<MindCore>> GetAllComponents();

	/**
	 * @brief Gets the attached Transform component.
	 */
	std::shared_ptr<Transform> GetTransform();

	/// Called once when the object is created.
	void Awake();

	/// Called once after Awake, before the game starts.
	void Start();

	/// Called every frame if the object is active.
	void Update();

	/// Called when the GameObject is destroyed.
	void OnDestroy();

	/**
	 * @brief Adds a child GameObject to this one.
	 */
	void AddChild(std::shared_ptr<GameObject> child);

	/**
	 * @brief Checks if the GameObject has a parent.
	 */
	bool HasParent();

	/// Draws debug visuals for components.
	void DrawGizmo();

	/// Renders visual content (e.g. sprites).
	void Render();

	/// Delete the desired components 
	void CheckForRemovableComponents();

	/**
	 * @brief Serializes the GameObject to JSON format.
	 */
	virtual void Serialize(nlohmann::json& json);

	/**
	 * @brief Deserializes the GameObject from JSON format.
	 */
	virtual void Deserialize(const nlohmann::json& json);

	/**
	 * @brief Gets a component of a specific type.
	 * @tparam T Component type.
	 */
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

	/**
	 * @brief Gets the first component of type T in children.
	 */
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

	/**
	 * @brief Gets all components of type T in children.
	 */
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

	/**
	 * @brief Gets a component of type T from the parent.
	 */
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

	/**
	 * @brief Returns if the object is currently active.
	 */
	bool IsActive() {
		return active_;
	}

	/**
	 * @brief Enables or disables this GameObject.
	 */
	void SetActive(bool active) {
		active_ = active;
	}

	/**
	 * @brief Removes a specific component from the GameObject.
	 */
	void RemoveComponent(const std::shared_ptr<MindCore> component);

	/**
	 * @brief Returns the UUID of the GameObject.
	 */
	std::string GetUUID();
private:
	/// List of all attached components.
	std::vector<std::shared_ptr<MindCore>> components_;

	/// Components queued for removal.
	std::vector<std::shared_ptr<MindCore>> to_remove_components_;

	/// Unique identifier of the object.
	std::string uuid_;

	/// Generates a UUID.
	void GenerateUUID();
};
