#pragma once

class GameObject;

class MindCore {
public:
	virtual void Awake();
	virtual void Start();

	virtual void Update();

	virtual void OnDestroy();

	virtual ~MindCore() = default;

	GameObject* game_object_ = nullptr;
};
