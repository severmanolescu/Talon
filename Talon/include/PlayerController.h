#pragma once

#include "MindCore.h"
#include "Rigidbody.h"
#include "Animator.h"

#include <memory>
#include <unordered_map>

/**
 * @brief Enumeration for character animation states.
 */
enum class AnimationState {
    Idle,
    Walk
};

/**
 * @brief Enumeration for character movement direction.
 */
enum class Direction {
    Up,
    Down,
    Left,
    Right
};

/**
 * @brief Hashing function for std::pair used in unordered_map.
 */
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

/**
 * @brief Player controller component that handles movement, jumping, animation state, and UI interaction.
 *
 * Uses Rigidbody for physics and AnimatorStateMachine to trigger animations.
 */
class PlayerController : public MindCore {
public:
    /// Movement speed.
    float walk_speed_ = 10.0f;

    /// Speed multiplier when sprinting.
    float sprint_speed_multiplier_ = 2.0f;

    /// Jump force applied when jumping.
    Vector2 jump_power_ = { .0f , -10.0f };

    /// Renderer used by Animator (if needed).
    SDL_Renderer* renderer_ = nullptr;

    PlayerController() {
        ui_frame_height_ = 8.0f;
    }

    /// Called when component is initialized.
    void Awake() override;

    /// Called before the first Update.
    void Start() override;

    /// Called every frame to update logic.
    void Update() override;

    /// Called when the component is destroyed.
    void OnDestroy() override;

    /// Renders this component's UI inside the inspector.
    void DrawUI() override;

    /// Serializes the component to JSON.
    void Serialize(nlohmann::json& json) override;

    /// Deserializes the component from JSON.
    void Deserialize(const nlohmann::json& json) override;

    /// Returns the name of the component type.
    const char* GetTypeName() const override { return "PlayerController"; }

    REGISTER_COMPONENT(PlayerController)

private:
    /// Reference to the Rigidbody for physics control.
    std::shared_ptr<Rigidbody> rigidbody_;

    /// Reference to the AnimatorStateMachine for animations.
    std::shared_ptr<AnimatorStateMachine> animator_state_machine_;

    /// Current animation state.
    AnimationState animation_state_ = AnimationState::Idle;

    /// Current direction based on input.
    Direction direction_ = Direction::Down;

    /// Last animation-direction pair to avoid redundant animation transitions.
    std::pair<AnimationState, Direction> last_state_;

    /**
     * @brief Sets variables in the AnimatorStateMachine based on the player's movement state.
     */
    void SetAnimatorVariables();
};
