#pragma once

#include "MindCore.h"

class PlayerController : public MindCore {
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};