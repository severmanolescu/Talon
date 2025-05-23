#pragma once

#include "Animator.h"

#include <variant>

struct AnimatorStateTransition;

/**
 * @brief Defines a condition that determines a transition between states or animations.
 */
struct TransitionCondition {
	std::string trigger;

	std::string condition_variable;
	std::variant<std::monostate, bool, int, float> expected_value;
};

/**
 * @brief Event that can be triggered during an animation frame.
 */
struct AnimationEvent {
	int frame = -1;
	std::function<void()> callback;
};

/**
 * @brief Represents a single animation with transitions and events.
 */
struct Animation {
	TransitionCondition contition;

	std::string name;

	std::string sprite_path;

	int columns = 0;
	int rows = 0;

	int frame_width = 0;
	int frame_height = 0;

	std::vector<AnimationEvent> events;
};

/**
 * @brief Represents a state within the animator state machine.
 */
struct AnimatorState {
	std::string name;

	std::vector<Animation> animations;

	std::vector<AnimatorStateTransition> transitions;
};

/**
 * @brief Defines a transition between two animator states.
 */
struct AnimatorStateTransition {
	TransitionCondition condition;

	AnimatorState* target_state = nullptr;
};

/**
 * @brief Controls state-driven animation logic through triggers, variables, and transitions.
 */
class AnimatorStateMachine : public MindCore {
public:
	AnimatorStateMachine() {
		priority = 13;
	}

	void Awake() override;
	void Update() override;

	/// State and animation management
	void AddState(const AnimatorState&);
	void SetState(const std::string&);
	void Trigger(const std::string&);

	void AddTriggerAnimatorState(const std::string& trigger, const std::string& source_state_name, const std::string& destination_state_name);
	void AddConditionAnimatorState(
		const std::string& condition_variable, 
		const std::variant<std::monostate, bool, int, float> expected_value,
		const std::string& source_state_name, 
		const std::string& destination_state_name);

	void AddAnimationToAnimator(const std::string &animator, const Animation &animation);
	void AddTriggerAnimation(const std::string& trigger, const std::string& animation_name, const std::string& animator_name);
	void AddContitionAnimation(
		const std::string& condition_variable,
		const std::variant<std::monostate, bool, int, float> expected_value,
		const std::string& animation_name,
		const std::string& animator_name);

	void AddTriggerCallback(const std::string animation_name, int frame, std::function<void()> callback);

	/// Condition variable setters/getters
	void SetBool(const std::string& name, bool value);
	bool GetBool(const std::string& name);

	void SetInt(const std::string& name, int value);
	int GetInt(const std::string& name);

	void SetFloat(const std::string& name, float value);
	float GetFloat(const std::string& name);

	void LoadFromJson(const std::string& path);

	/// Serialization
	void Serialize(nlohmann::json& json) override;
	void Deserialize(const nlohmann::json& json) override;

	/// Returns the name of the component type.
	const char* GetTypeName() const override { return "AnimatorStateMachine"; }

	REGISTER_COMPONENT(AnimatorStateMachine)
private:
	std::shared_ptr<Animator> animator_;

	std::unordered_map<std::string, AnimatorState> states_;

	std::unordered_map<std::string, bool> bool_conditions_;
	std::unordered_map<std::string, int> int_conditions_;
	std::unordered_map<std::string, float> float_conditions_;

	std::string current_state_;

	std::string current_animation_name_;

	std::string config_path_;

	void SetAnimation(Animation animation);
};