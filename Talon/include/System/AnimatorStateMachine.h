#pragma once

#include "Animator.h"

struct AnimatorState {
	std::string name;

	std::string sprite_path;
	int columns = 0;
	int rows = 0;

	// Next animation based on trigger
	std::unordered_map<std::string, std::string> transitions;
};

class AnimatorStateMachine : public MindCore {
public:
	void Awake() override;

	void AddState(const AnimatorState&);
	void SetState(const std::string&);
	void Trigger(const std::string&);
private:
	std::shared_ptr<Animator> animator_;

	std::unordered_map<std::string, AnimatorState> states_;

	std::string current_state_;
};