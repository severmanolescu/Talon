#include "MindEngine.h"

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> MindEngine::game_objects_ = 
std::make_shared<std::vector<std::shared_ptr<GameObject>>>();

std::shared_ptr<GameObject> MindEngine::FindInChildren(const std::shared_ptr<GameObject>& obj, const std::string& name){
    if (obj->name_ == name) return obj;

    for (const auto& child : obj->childrens_) {
        auto found = FindInChildren(child, name);
        if (found) return found;
    }

    return nullptr;
}

std::shared_ptr<GameObject> MindEngine::FindGameObject(const std::string& name){
    for (const auto& root : *game_objects_) {
        auto found = FindInChildren(root, name);
        if (found) return found;
    }
    return nullptr;
}

void MindEngine::RemoveGameObject(const std::shared_ptr<GameObject>& object) {
    if (!object) return;

    if (auto parent = object->parent_.lock()) {
        auto& siblings = parent->childrens_;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), object), siblings.end());
    }
    else {
        game_objects_->erase(std::remove(game_objects_->begin(), game_objects_->end(), object), game_objects_->end());
    }
}

void MindEngine::RemoveAllGameObjects(){
    game_objects_->clear();
}

void MindEngine::AddGameObject(const std::shared_ptr<GameObject>& game_object){
	if (game_objects_) {
		game_objects_->push_back(game_object);
	}
}

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> MindEngine::GetAllGameObjects(){
    return game_objects_;
}
