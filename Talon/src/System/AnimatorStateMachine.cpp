#include "AnimatorStateMachine.h"
#include "GameObject.h"
#include "Console.hpp"

#include <json.hpp>
#include <fstream>

#include "iostream"

void AnimatorStateMachine::Awake() {
	animator_ = game_object_->GetComponent<Animator>();
}

void AnimatorStateMachine::AddState(const AnimatorState& state) {
	states_[state.name] = state;
}

void AnimatorStateMachine::AddTriggerAnimatorState(const std::string& trigger, const std::string& source_state_name, const std::string& destination_state_name) {
	if (states_.empty() ||
		states_.find(source_state_name) == states_.end() ||
		states_.find(destination_state_name) == states_.end()) {
		return;
	}

	AnimatorState& source_state = states_[source_state_name];
	AnimatorState& destination_state = states_[destination_state_name];

	TransitionCondition condition({trigger, "", std::monostate()});

	source_state.transitions.push_back({ 
		condition,
		&destination_state 
	});
}

void AnimatorStateMachine::AddConditionAnimatorState(
	const std::string& condition_variable,
	const std::variant<std::monostate, bool, int, float> expected_value,
	const std::string& source_state_name,
	const std::string& destination_state_name)
{
	if (states_.empty() ||
		states_.find(source_state_name) == states_.end() ||
		states_.find(destination_state_name) == states_.end()) {
		return;
	}

	AnimatorState& source_state = states_[source_state_name];
	AnimatorState& destination_state = states_[destination_state_name];

	TransitionCondition condition({ "", condition_variable, expected_value});

	source_state.transitions.push_back({
		condition,
		&destination_state
	});
}

void AnimatorStateMachine::AddAnimationToAnimator(const std::string& animator, const Animation& animation){
	if (states_.empty() || states_.find(animator) == states_.end()) {
		return;
	}
	AnimatorState& state = states_[animator];
	state.animations.push_back(animation);
}

void AnimatorStateMachine::AddTriggerAnimation(const std::string& trigger, const std::string& animation_name, const std::string& animator_name) {
	if (states_.empty() || states_.find(animator_name) == states_.end()) {
		return;
	}
	AnimatorState& state = states_[animator_name];
	for (auto& animation : state.animations) {
		if (animation.name == animation_name) {
			animation.contition.trigger = trigger;

			break;
		}
	}
}

void AnimatorStateMachine::AddContitionAnimation(
	const std::string& condition_variable, 
	const std::variant<std::monostate, bool, int, float> expected_value, 
	const std::string& animation_name, 
	const std::string& animator_name){
	if (states_.empty() || states_.find(animator_name) == states_.end()) {
		return;
	}
	AnimatorState& state = states_[animator_name];
	for (auto& animation : state.animations) {
		if (animation.name == animation_name) {
			animation.contition.condition_variable = condition_variable;
			animation.contition.expected_value = expected_value;

			break;
		}
	}
}

void AnimatorStateMachine::AddTriggerCallback(const std::string animation_name, int frame, std::function<void()> callback){
	if (states_.empty()) {
		return;
	}
	for (auto& state : states_) {
		for (auto& animation : state.second.animations) {
			if (animation.name == animation_name) {
				animation.events.push_back({ frame, callback });
				break;
			}
		}
	}
}

void AnimatorStateMachine::SetBool(const std::string& name, bool value) {
	bool_conditions_[name] = value;
}

bool AnimatorStateMachine::GetBool(const std::string& name) {
	if (bool_conditions_.find(name) != bool_conditions_.end()) {
		return bool_conditions_[name];
	}
	return false;
}

void AnimatorStateMachine::SetInt(const std::string& name, int value) {
	int_conditions_[name] = value;
}

int AnimatorStateMachine::GetInt(const std::string& name) {
	if (int_conditions_.find(name) != int_conditions_.end()) {
		return int_conditions_[name];
	}
	return 0;
}

void AnimatorStateMachine::SetFloat(const std::string& name, float value) {
	float_conditions_[name] = value;
}

float AnimatorStateMachine::GetFloat(const std::string& name) {
	if (float_conditions_.find(name) != float_conditions_.end()) {
		return float_conditions_[name];
	}
	return .0f;
}

void AnimatorStateMachine::SetAnimation(Animation animation){
	if (current_animation_name_ == animation.name) {
		return;
	}

	current_animation_name_ = animation.name;

	animator_->spritesheet_columns_ = animation.columns;
	animator_->spritesheet_rows = animation.rows;

	animator_->frame_width_ = animation.frame_width;
	animator_->frame_height_ = animation.frame_height;

	animator_->SetSpriteSheet(animation.sprite_path, animator_->GetRender());
}

void AnimatorStateMachine::SetState(const std::string& state_name) {
	if (state_name.compare(current_state_) != 0 && states_.find(state_name) != states_.end()) {
		current_state_ = state_name;

		for (const auto& animation : states_[state_name].animations) {
			if (std::holds_alternative<std::monostate>(animation.contition.expected_value) && animation.contition.trigger.empty()) {
				SetAnimation(animation);
				break;
			}
		}
	}
}

void AnimatorStateMachine::Trigger(const std::string& trigger) {
	const auto& state = states_[current_state_];

	for (const auto& transition : state.transitions) {
		if (transition.condition.trigger == trigger && transition.target_state != nullptr) {
			SetState(transition.target_state->name);
			return;
		}
	}

	for (auto& animation : state.animations) {
		if (animation.contition.trigger == trigger) {
			SetAnimation(animation);
			return;
		}
	}
}

void AnimatorStateMachine::Update() {
	const auto& state = states_[current_state_];

	for (const auto& transition : state.transitions) {
		if (std::holds_alternative<bool>(transition.condition.expected_value)) {
			if (GetBool(transition.condition.condition_variable) == std::get<bool>(transition.condition.expected_value)) {
				SetState(transition.target_state->name);
				break;
			}
		}
		else if (std::holds_alternative<int>(transition.condition.expected_value)) {
			if (GetInt(transition.condition.condition_variable) == std::get<int>(transition.condition.expected_value)) {
				SetState(transition.target_state->name);
				break;
			}
		}
		else if (std::holds_alternative<float>(transition.condition.expected_value)) {
			if (GetFloat(transition.condition.condition_variable) == std::get<float>(transition.condition.expected_value)) {
				SetState(transition.target_state->name);
				break;
			}
		}
	}

	for (auto& animation : state.animations) {
		if (animation.contition.condition_variable != "" && animation.contition.expected_value.index() != 0) {
			if (std::holds_alternative<bool>(animation.contition.expected_value)) {
				if (GetBool(animation.contition.condition_variable)) {
					SetAnimation(animation);
					return;
				}
			}
			else if (std::holds_alternative<int>(animation.contition.expected_value)) {
				if (GetInt(animation.contition.condition_variable) == std::get<int>(animation.contition.expected_value)) {
					SetAnimation(animation);
					return;
				}
			}
			else if (std::holds_alternative<float>(animation.contition.expected_value)) {
				if (GetFloat(animation.contition.condition_variable) == std::get<float>(animation.contition.expected_value)) {
					SetAnimation(animation);
					return;
				}
			}
		}
	}
}

void AnimatorStateMachine::LoadFromJson(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		LOG_ERROR("[AnimatorStateMachine] Failed to open file: " + path);
		return;
	}

	config_path_ = path;

	nlohmann::json json_data;
	file >> json_data;

	for (const auto& state_json : json_data["states"]) {
		AnimatorState state;
		state.name = state_json["name"];

		for (const auto& anim_json : state_json["animations"]) {
			Animation animator;

			animator.name = anim_json["name"];
			animator.sprite_path = anim_json["sprite_path"];

			animator.columns = anim_json["columns"];
			animator.rows = anim_json["rows"];

			animator.frame_width = anim_json["frame_width"];
			animator.frame_height = anim_json["frame_height"];

			if (anim_json.contains("condition")) {
				const auto& cond = anim_json["condition"];
				animator.contition.condition_variable = cond["variable"];

				const auto& val = cond["value"];
				std::string type = val["type"];

				if (type == "bool") animator.contition.expected_value = val["data"].get<bool>();
				else if (type == "int") animator.contition.expected_value = val["data"].get<int>();
				else if (type == "float") animator.contition.expected_value = val["data"].get<float>();
			}

			state.animations.push_back(animator);
		}

		AddState(state);
	}

	// Load transitions
	for (const auto& trans : json_data["transitions"]) {
		std::string from = trans["from"];
		std::string to = trans["to"];

		const auto& condition = trans["condition"];
		std::string variable = condition["variable"];

		std::variant<std::monostate, bool, int, float> value;

		const auto& val = condition["value"];
		std::string type = val["type"];
		if (type == "bool") value = val["data"].get<bool>();
		else if (type == "int") value = val["data"].get<int>();
		else if (type == "float") value = val["data"].get<float>();
		else continue;

		AddConditionAnimatorState(variable, value, from, to);
	}
}

void AnimatorStateMachine::Serialize(nlohmann::json& json){
	nlohmann::json animator_state_machine;

	animator_state_machine["type"] = "AnimatorStateMachine";

	animator_state_machine["data"]["config path"] = config_path_;

	animator_state_machine["active"] = active_;

	json.push_back(animator_state_machine);
}

void AnimatorStateMachine::Deserialize(const nlohmann::json& json){
	if (json.contains("config path")) {
		LoadFromJson(json["config path"]);
	}
}
