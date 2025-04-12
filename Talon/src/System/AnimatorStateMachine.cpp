#include "AnimatorStateMachine.h"
#include "GameObject.h"

void AnimatorStateMachine::Awake() {
	animator_ = game_object_->GetComponent<Animator>();
}

void AnimatorStateMachine::AddState(const AnimatorState& state) {
	states_[state.name] = state;
}

void AnimatorStateMachine::SetState(const std::string& state_name) {
	if (states_.find(state_name) != states_.end()) {
		current_state_ = state_name;

		const AnimatorState& state = states_[state_name];

		animator_->columns_ = state.columns;
		animator_->rows_ = state.rows;
		animator_->SetSpriteSheet(state.sprite_path, animator_->GetRender());
	}
}

void AnimatorStateMachine::Trigger(const std::string& trigger) {
	const auto& state = states_[current_state_];
	
	if (state.transitions.empty()) return;

	auto it = state.transitions.find(trigger);

	if (it != state.transitions.end()) {
		SetState(it->second);
	}
}