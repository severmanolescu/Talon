#pragma once

#include "MindCore.h"
#include "Transform.h"

#include "memory"


/**
 * @brief Default gravitational acceleration used by Rigidbody.
 */
constexpr float DEFAULT_GRAVITY = 9.81f;

/**
 * @brief Clamps a value between a minimum and a maximum.
 *
 * @tparam T Numeric type
 * @param value The input value.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The clamped value.
 */
template <typename T>
T clamp(T value, T min, T max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

/// @addtogroup Components
/// @{

/**
 * @class Rigidbody
 * @brief Simulates 2D physics for a GameObject.
 *
 * Applies forces, gravity, and collisions to objects. This component interacts
 * with the physics system and requires a Transform component.
 *
 */
class Rigidbody : public MindCore {
public:
	/**
	 * @brief Constructor that sets the component execution priority.
	 */
	Rigidbody() {
		priority = 10;
	}

	/// Current velocity of the GameObject.
	Vector2 velocity_ = { .0f, .0f };
	/// Acceleration applied this frame.
	Vector2 acceleration_ = { .0f, .0f };

	/// Simulates drag along X and Y axes.
	Vector2 linear_drag_ = {.9f, .9f};
	
	/// Mass of the GameObject (affects applied force).
	float mass_ = 1.0f;

	/// Gravitational acceleration.
	float gravity_ = DEFAULT_GRAVITY;

	/// Maximum allowed velocity.
	float max_velocity_ = 50.0f;

	/// Enables or disables gravity.
	bool use_gravity_ = true;

	/// Kinematic bodies are not affected by physics forces.
	bool is_kinematic_ = false;

	/**
	 * @brief Fetches required components and initializes the Rigidbody.
	 */
	void Awake() override;

	/**
	 * @brief Applies a continuous force to the Rigidbody.
	 * @param force The force vector.
	 */
	void AddForce(const Vector2& force);

	/**
	 * @brief Instantly sets the velocity.
	 * @param new_velocity The new velocity vector.
	 */
	void SetVelocity(const Vector2& new_velocity);

	/**
	 * @brief Adds a delta velocity to the current velocity.
	 * @param delta_velocity The velocity change to apply.
	 */
	void AddVelocity(const Vector2& delta_velocity);

	/**
	 * @brief Applies an instant impulse force.
	 * @param impulse The impulse to apply.
	 */
	void ApplyImpulse(const Vector2& impulse);

	/**
	 * @brief Called when a collision is detected.
	 */
	void OnCollision();

	/**
	 * @brief Checks for and resolves any collisions.
	 */
	void CheckAndResolveCollision();

	/**
	 * @brief Updates the Rigidbody state every frame.
	 */
	void Update() override;

	/**
	 * @brief Renders editable Rigidbody properties in the Inspector.
	 */
	void DrawUI() override;

	/**
	 * @brief Serializes the component into a JSON object.
	 * @param json Target JSON to write into.
	 */
	void Serialize(nlohmann::json& json) override;

	/**
	 * @brief Loads the component from a JSON object.
	 * @param json Source JSON to read from.
	 */
	void Deserialize(const nlohmann::json& json) override;

	/// Returns the name of the component type.
	const char* GetTypeName() const override { return "Rigidbody"; }

	REGISTER_COMPONENT(Rigidbody)

private:
	/// Reference to the Transform component of the GameObject.
	std::shared_ptr<Transform> transform_;
};

/// @}
