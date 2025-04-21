#pragma once

#include "GameObject.h"

/**
 * @class MindEngine
 * @brief Central manager for all GameObjects in the scene.
 *
 * This class acts as the scene-level controller, providing access
 * to GameObject creation, lookup, and removal.
 *
 * It manages the top-level GameObjects (those without parents) and
 * provides utilities to access all active objects in the current scene.
 */
class MindEngine {
public:
	/**
	 * @brief Find a GameObject by name.
	 *
	 * @param name The name of the GameObject to find.
	 * @return Shared pointer to the found GameObject, or nullptr if not found.
	 */
	static std::shared_ptr<GameObject> FindGameObject(const std::string& name);

	/**
	 * @brief Recursively find a GameObject by name starting from a given GameObject.
	 *
	 * @param obj The GameObject to start searching from.
	 * @param name The name of the GameObject to find.
	 * @return Shared pointer to the found GameObject, or nullptr if not found.
	 */
	static std::shared_ptr<GameObject> FindInChildren(const std::shared_ptr<GameObject>& obj, const std::string& name);

	/**
	 * @brief Remove a specific GameObject from the scene.
	 *
	 * If the GameObject has a parent, it will be removed from the parent's children list.
	 * Otherwise, it will be removed from the root scene list.
	 *
	 * @param object The GameObject to remove.
	 */
	static void RemoveGameObject(const std::shared_ptr<GameObject>& object);

	/**
	 * @brief Remove all GameObjects from the scene.
	 *
	 * Clears the entire scene graph.
	 */
	static void RemoveAllGameObjects();

	/**
	 * @brief Add a new GameObject to the scene.
	 *
	 * If the GameObject has no parent, it is added to the root list.
	 *
	 * @param game_object The GameObject to add.
	 */
	static void AddGameObject(const std::shared_ptr<GameObject>& game_object);

	/**
	 * @brief Get all root-level GameObjects in the current scene.
	 *
	 * @return Shared pointer to a vector of GameObjects.
	 */
	static std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GetAllGameObjects();
private:
	/**
	 * @brief List of all root-level GameObjects in the scene.
	 */
	static std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> game_objects_;
};