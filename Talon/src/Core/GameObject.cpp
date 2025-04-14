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
	if (!active_) return;

	to_remove_components_.clear();

	for (auto& component : components_) {
		component->Awake();
	}
}

void GameObject::Start() {
	if (!active_) return;

	for (auto& component : components_) {
		component->Start();
	}
}

void GameObject::Update() {
	if (!active_) return;

	for (auto& component : components_) {
		component->Update();
	}

	for (auto& component : to_remove_components_) {
		auto it = std::remove(components_.begin(), components_.end(), component);
		if (it != components_.end()) {
			components_.erase(it, components_.end());
		}
	}
}

void GameObject::OnDestroy() {
	for (auto& component : components_) {
		component->OnDestroy();
	}
}

void GameObject::AddChild(std::shared_ptr<GameObject> child){
	childrens_.push_back(child);
	child->parent_ = shared_from_this();
}

bool GameObject::HasParent(){
	return !parent_.expired();

	return false;
}

void GameObject::RemoveComponent(const std::shared_ptr<MindCore> component){
	to_remove_components_.push_back(component);
}
