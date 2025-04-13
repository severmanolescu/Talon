#include "GameObject.h"

#include "AnimatorStateMachine.h"
#include "Animator.h"

void GameObject::AddComponent(const std::shared_ptr<MindCore> component) {
	component->game_object_ = this;
	components_.push_back(component);
}

std::vector<std::shared_ptr<MindCore>> GameObject::GetAllComponents() {
	return components_;
}

std::shared_ptr<Transform> GameObject::GetTransform() {
	return GetComponent<Transform>();
}

void GameObject::Awake() {
	for (auto& component : components_) {
		component->Awake();
	}
}

void GameObject::Start() {
	for (auto& component : components_) {
		component->Start();
	}
}

void GameObject::Update() {
	for (auto& component : components_) {
		component->Update();
	}
}

void GameObject::OnDestroy() {
	for (auto& component : components_) {
		component->OnDestroy();
	}
}