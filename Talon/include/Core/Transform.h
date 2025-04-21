#pragma once

#include "MindCore.h"
#include "Vector2.h"

#include "memory"

/**
 * @class Transform
 * @brief Handles position, scale, and rotation of a GameObject in world space.
 *
 * The Transform component is fundamental and required by all GameObjects.
 * It defines the spatial properties and provides utility to compute world position
 * even in a hierarchy.
 */
class Transform : public MindCore {
public:
	/**
	 * @brief Default constructor. Sets initial priority and default scale.
	 */
	Transform() {
		priority = 0;
	}

	/**
	 * @brief Position of the object in local space.
	 */
	Vector2 position_;

	/**
	 * @brief Cached relative position to the parent object.
	 */
	Vector2 relative_position;

	/**
	 * @brief Scale of the object in local space.
	 */
	Vector2 scale_ = { 1.0f, 1.0f };

	/**
	 * @brief Rotation of the object in degrees.
	 */
	float rotation_ = .0f;

	/**
	 * @brief Calculates the world position by recursively checking the parent.
	 * @return The world position of the object.
	 */
	Vector2 GetWorldPosition();

	/**
	 * @brief Sets the world position and updates the local position accordingly.
	 * @param world_pos Target world-space position.
	 */
	void SetWorldPosition(const Vector2& world_pos);

	/**
	 * @brief Draws this component in the ImGui Inspector panel.
	 */
	void DrawUI() override;

	/**
	 * @brief Serializes the Transform component into JSON.
	 * @param json JSON object to write to.
	 */
	void Serialize(nlohmann::json& json) override;

	/**
	 * @brief Deserializes the Transform component from JSON.
	 * @param json JSON object to read from.
	 */
	void Deserialize(const nlohmann::json& json) override;

	/// Returns the name of the component type.
	const char* GetTypeName() const override { return "Transform"; }

	/**
	 * @brief Macro to register this component with the factory.
	 */
	REGISTER_COMPONENT(Transform)
};
