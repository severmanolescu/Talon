#pragma once

#include "MindCore.h"
#include "Rigidbody.h"
#include "Animator.h"

#include "memory"

#include <unordered_map>

enum class AnimationState {
	Idle,
	Walk
};

enum class Direction {
	Up,
	Down,
	Left,
	Right
};

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& p) const {
		return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
	}
};

class PlayerController : public MindCore {
public:
	float walk_speed_ = 10.0f;
	float sprint_speed_multiplier_ = 2.0f;

	Vector2 jump_power_ = { .0f , -10.0f };

	SDL_Renderer* renderer_ = nullptr;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void DrawUI() override;

	void Serialize(nlohmann::json& json) override;
	void Deserialize(const nlohmann::json& json) override;

	REGISTER_COMPONENT(PlayerController)

private:
	std::shared_ptr<Rigidbody> rigidbody_;
	std::shared_ptr<AnimatorStateMachine> animator_state_machine_;

	AnimationState animation_state_ = AnimationState::Idle;
	Direction direction_ = Direction::Down;

	std::pair<AnimationState, Direction> last_state_;

	void SetAnimatorVariables();
};
