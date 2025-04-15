#include "GameObject.h"

#include "AnimatorStateMachine.h"
#include "Animator.h"

#include <algorithm>

void GameObject::AddComponent(const std::shared_ptr<MindCore> component) {
	component->game_object_ = this;
	components_.push_back(component);

	std::sort(components_.begin(), components_.end(), [](const auto& a, const auto& b) {
		return a->GetExecutionPriority() < b->GetExecutionPriority();
		});
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

	for (auto& child : childrens_) {
		child->Awake();
	}
}

void GameObject::Start() {
	if (!active_) return;

	for (auto& component : components_) {
		component->Start();
	}

	for (auto& child : childrens_) {
		child->Start();
	}
}

void GameObject::Update() {
	if (!active_) return;

	for (auto& component : components_) {
		component->Update();
	}

	for (auto& child : childrens_) {
		child->Update();
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

void GameObject::DrawGizmo(){
	for (auto& component : components_) {
		component->DrawGizmo();
	}

	for (auto& child : childrens_) {
		child->DrawGizmo();
	}
}

void GameObject::Render(){
	std::shared_ptr<SpriteRenderer> renderer = GetComponent<SpriteRenderer>();
	if (renderer) {
		renderer->Render();
	}

	for (auto& child : childrens_) {
		child->Render();
	}
}

void GameObject::RemoveComponent(const std::shared_ptr<MindCore> component){
	to_remove_components_.push_back(component);
}
