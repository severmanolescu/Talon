#include "GameObject.h"

#include "AnimatorStateMachine.h"
#include "Animator.h"

#include <algorithm>
#include <random>
#include <sstream>

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

	CheckForRemovableComponents();
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

	CheckForRemovableComponents();
}

void GameObject::CheckForRemovableComponents(){
	for (auto& component : to_remove_components_) {
		auto it = std::remove(components_.begin(), components_.end(), component);
		if (it != components_.end()) {
			components_.erase(it, components_.end());
		}
	}

	to_remove_components_.clear();
}

void GameObject::Serialize(nlohmann::json& json)
{
	nlohmann::json object_data;

	object_data["name"] = name_;
	object_data["active"] = active_;
	object_data["uuid"] = uuid_;

	if (auto parent = parent_.lock()) {
		object_data["parent"] = parent->GetUUID();
	}

	object_data["components"] = nlohmann::json::array();


	for (auto& component : components_) {
		component->Serialize(object_data["components"]);
	}

	json.push_back(object_data);

	for (auto& child : childrens_) {
		child->Serialize(json);
	}
}

void GameObject::Deserialize(const nlohmann::json& json)
{
}

void GameObject::RemoveComponent(const std::shared_ptr<MindCore> component){
	to_remove_components_.push_back(component);
}

std::string GameObject::GetUUID(){
	return uuid_;
}

void GameObject::GenerateUUID(){
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);

	std::stringstream ss;
	for (int i = 0; i < 32; ++i) {
		ss << std::hex << dis(gen);
	}
	uuid_ = ss.str();
}
