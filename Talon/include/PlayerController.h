#pragma once

#include "MindCore.h"
#include "Rigidbody.h"
#include "BoxCollider.h"

#include "memory"

class PlayerController : public MindCore {
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	float walk_speed_ = 1.0f;
	float sprint_speed_ = 2.0f;

	Vector2 jump_power_ = {.0f , -10.0f};

private:
	std::shared_ptr<Rigidbody> rigidbody_;
	std::shared_ptr<BoxCollider> box_collider_;
};
