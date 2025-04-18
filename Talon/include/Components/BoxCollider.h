#pragma once

#include <SDL2/SDL.h>

#include "MindCore.h"
#include "Transform.h"

#include "memory"

/// @addtogroup Components
/// @{

/**
 * @class BoxCollider
 * @brief A 2D rectangular collider for handling collision detection.
 *
 * This component defines the collision bounds of a GameObject.
 * It supports visualization for debugging and can be configured
 * via the Inspector.
 *
 */
class BoxCollider : public MindCore {
public:
	/**
	 * @brief Constructs a BoxCollider and sets the execution priority.
	 */
	BoxCollider() {
		priority = 5;
	}

	/// Width of the collider.
	int width_ = 50;
	/// Height of the collider.
	int height_ = 50;

	/// Offset from the GameObject's Transform position.
	Vector2 offset_ = { .0f, .0f };

	/// Whether to render the collider in debug mode.
	bool draw_debug_ = false;

	/**
	 * @brief Gets the SDL_Rect representing the collider's bounds in world space.
	 * @return SDL_Rect representing the position and size.
	 */
	SDL_Rect GetBounds() const;

	/**
	 * @brief Initializes the component and fetches the required Transform.
	 */
	void Awake() override;

	/**
	 * @brief Renders the collider as a visual outline (for debugging).
	 */
	void DrawGizmo() override;

	/**
	 * @brief Displays editable properties in the Inspector panel.
	 */
	void DrawUI() override;

	/**
	 * @brief Serializes component data into JSON format.
	 * @param json The target JSON object.
	 */
	void Serialize(nlohmann::json& json) override;

	/**
	 * @brief Loads component data from JSON format.
	 * @param json The source JSON object.
	 */
	void Deserialize(const nlohmann::json& json) override;

	REGISTER_COMPONENT(BoxCollider)

private:
	/// Reference to the owning Transform component.
	std::shared_ptr<Transform> transform_;
};

/// @}