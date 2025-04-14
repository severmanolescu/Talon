#include "HierarchyPanel.h"
#include "MindEngine.h"
#include "Console.hpp"

#include "algorithm"
#include "imgui.h"
#include "cstring"

//TO-DO: Double click on the GameObject to center the camera on it
//TO-DO: Drag and drop to the project panel to create a prefab
//TO-DO: Save the setings of the panel to the json

bool HierarchyPanel::NameMatchesSearch(const std::string& name, const char* search) {
    if (!search || std::strlen(search) == 0) return true;

    std::string lower_name = name;
    std::string lower_search = search;

    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    std::transform(lower_search.begin(), lower_search.end(), lower_search.begin(), ::tolower);

    return lower_name.find(lower_search) != std::string::npos;
}

std::string HierarchyPanel::GenerateUniqueName(const std::string& base_name, const std::shared_ptr<GameObject>& parent){
    int counter = 1;
    std::string new_name = base_name;

    auto has_name = [&](const std::string& name) {
        for (const auto& child : parent->childrens_) {
            if (child->name_ == name)
                return true;
        }
        return false;
        };

    while (has_name(new_name)) {
        new_name = base_name + " (" + std::to_string(counter++) + ")";
    }

    return new_name;
}

bool IsDescendantOf(const std::shared_ptr<GameObject>& parent, const std::shared_ptr<GameObject>& possible_child) {
    for (const auto& child : parent->childrens_) {
        if (child == possible_child || IsDescendantOf(child, possible_child))
            return true;
    }
    return false;
}

void HierarchyPanel::HandleUserKeyboardInput(const std::shared_ptr<GameObject>& object) {
    bool is_selected = std::find(selected_objects_.begin(), selected_objects_.end(), object) != selected_objects_.end();

    bool ctrl = ImGui::GetIO().KeyCtrl;

    if (ImGui::IsItemClicked()) {
        auto& io = ImGui::GetIO();

        if (io.KeyShift && last_clicked_object_) {
            auto it1 = std::find(visible_flat_list_.begin(), visible_flat_list_.end(), last_clicked_object_);
            auto it2 = std::find(visible_flat_list_.begin(), visible_flat_list_.end(), object);

            if (it1 != visible_flat_list_.end() && it2 != visible_flat_list_.end()) {
                if (it2 < it1) std::swap(it1, it2);

                selected_objects_.clear();
                for (auto it = it1; it <= it2; ++it) {
                    selected_objects_.push_back(*it);
                }
            }
            last_clicked_object_ = object;
        }

        else if (io.KeyCtrl) {
            if (is_selected) {
                selected_objects_.erase(std::remove(selected_objects_.begin(), selected_objects_.end(), object), selected_objects_.end());
            }
            else {
                selected_objects_.push_back(object);
            }
            last_clicked_object_ = object;
        }
        else {
            selected_objects_.clear();
            selected_objects_.push_back(object);
            last_clicked_object_ = object;
        }
    }

    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload(hierarchy_gameobject_identifier, &object, sizeof(std::shared_ptr<GameObject>));
        ImGui::Text("Move %s", object->name_.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
        if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
            for (const auto& obj : selected_objects_) {
                MindEngine::RemoveGameObject(obj);
            }
            selected_objects_.clear();
        }

        bool ctrl = ImGui::GetIO().KeyCtrl;

        if (ctrl && ImGui::IsKeyPressed(ImGuiKey_D)) {
            for (const auto& obj : selected_objects_) {
                auto new_object = std::make_shared<GameObject>(obj->name_ + " (Copy)");
                if (auto parent = obj->parent_.lock()) {
                    parent->AddChild(new_object);
                }
                else {
                    MindEngine::AddGameObject(new_object);
                }
            }
        }
    }
}

void HierarchyPanel::DrawPopup(const std::shared_ptr<GameObject>& object){
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Copy Name")) {
            ImGui::SetClipboardText(object->name_.c_str());
        }
        else if (ImGui::Selectable("Rename")) {
            renaming_target_ = object;

            strncpy_s(rename_buffer_, sizeof(rename_buffer_), object->name_.c_str(), _TRUNCATE);
            rename_buffer_[sizeof(rename_buffer_) - 1] = '\0';

            ImGui::SetKeyboardFocusHere();
        }
        else if (ImGui::Selectable("Delete")) {
            delete_selected_ = true;
        }
        else if (ImGui::Selectable("Add Empty Child")) {
            std::string child_name = GenerateUniqueName(new_game_object_name, object);

            auto child = std::make_shared<GameObject>(child_name);
            object->AddChild(child);

        }
        else if (ImGui::Selectable("Copy Full Path")) {
            std::string path = object->name_;
            auto current = object->parent_;
            while (!current.expired()) {
                path = current.lock()->name_ + "/" + path;
                current = current.lock()->parent_;
            }
            ImGui::SetClipboardText(path.c_str());
        }
        else if (ImGui::Selectable("Unparent")) {
            pending_reparent_object_ = object;
            pending_reparent_target_ = nullptr;
        }
        ImGui::EndPopup();
    }
}

void HierarchyPanel::RenameObject(const std::shared_ptr<GameObject>& object) {
    ImGui::PushID(object.get());
    if (ImGui::InputText("##Rename", rename_buffer_, IM_ARRAYSIZE(rename_buffer_),
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {

        std::string trimmed = rename_buffer_;
        trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
        trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

        if (!trimmed.empty()) {
            for (auto& selected_object : selected_objects_) {
                selected_object->name_ = trimmed;
            }
        }
        else {
            LOG_WARNING("GameObject name cannot be empty: " + object->name_);
        }
    }

    if (ImGui::IsItemDeactivatedAfterEdit()) {
        renaming_target_ = nullptr;
    }

    ImGui::PopID();
}
void HierarchyPanel::DrawDropZoneBelow(std::shared_ptr<GameObject> object) {
    ImGui::PushID(("drop_below_" + object->name_).c_str());
    ImGui::InvisibleButton("##drop_below", ImVec2(ImGui::GetContentRegionAvail().x, drop_zone_height));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(hierarchy_gameobject_identifier)) {
            auto dropped = *(std::shared_ptr<GameObject>*)payload->Data;

            if (dropped != object && dropped->parent_.lock() == object->parent_.lock()) {
                pending_reorder_object_ = dropped;
                reorder_target_ = object;
                reorder_above_ = false;
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::PopID();
}
void HierarchyPanel::DrawDropZoneAbove(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> scene) {
    ImGui::PushID("drop_zone_before_first");
    ImGui::InvisibleButton("##drop_zone_top", ImVec2(ImGui::GetContentRegionAvail().x, drop_zone_height));
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(hierarchy_gameobject_identifier)) {
            auto dropped = *(std::shared_ptr<GameObject>*)payload->Data;

            // Only allow dropping here if it's the same parent (or root)
            if (!dropped->parent_.lock()) {
                pending_reorder_object_ = dropped;
                reorder_target_ = scene->front();
                reorder_above_ = true;
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::PopID();
}

void HierarchyPanel::DrawGameObjectNode(const std::shared_ptr<GameObject>& object) {
    bool matches = NameMatchesSearch(object->name_, search_buffer_);
    visible_flat_list_.push_back(object);

    bool hasMatchingChild = false;
    for (const auto& child : object->childrens_) {
        if (NameMatchesSearch(child->name_, search_buffer_)) {
            hasMatchingChild = true;
            break;
        }
    }

    if (!matches && !hasMatchingChild) return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool is_selected = std::find(selected_objects_.begin(), selected_objects_.end(), object) != selected_objects_.end();
    if (is_selected)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool has_children = !object->childrens_.empty();
    bool open = false;

    if (renaming_target_ == object) {
		RenameObject(object);
        
        open = false;
    }
    else {
        if (has_children) {
            open = ImGui::TreeNodeEx((void*)object.get(), flags, "%s", object->name_.c_str());
        }
        else {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)object.get(), flags, "%s", object->name_.c_str());
        }
    }

    HandleUserKeyboardInput(object);

    DrawPopup(object);

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(hierarchy_gameobject_identifier)) {
            auto dropped = *(std::shared_ptr<GameObject>*)payload->Data;

            if (dropped != object && !IsDescendantOf(dropped, object)) {
                pending_reparent_object_ = dropped;
                pending_reparent_target_ = object;
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (open) {
        for (auto& child : object->childrens_) {
            DrawGameObjectNode(child);
        }
        ImGui::TreePop();
    }

	DrawDropZoneBelow(object);
}

void HierarchyPanel::BuildFlatList(const std::vector<std::shared_ptr<GameObject>>& objects) {
    for (const auto& obj : objects) {
        if (NameMatchesSearch(obj->name_, search_buffer_)) {
            visible_flat_list_.push_back(obj);
        }
        BuildFlatList(obj->childrens_);
    }
}
void HierarchyPanel::BeginPanel() {
    ImGui::Begin("Hierarchy");

    ImGui::InputTextWithHint("##Search", "Search GameObject...", search_buffer_, IM_ARRAYSIZE(search_buffer_));
    ImGui::Separator();
}

void HierarchyPanel::HandlePendingActions() {

    if (delete_selected_) {
        for (const auto& obj : selected_objects_) {
            MindEngine::RemoveGameObject(obj);
        }
    }

    if (pending_duplicate_) {
        auto new_object = std::make_shared<GameObject>(pending_duplicate_->name_ + " (Copy)");

        if (auto parent = pending_duplicate_->parent_.lock()) {
            parent->AddChild(new_object);
        }
        else {
            MindEngine::AddGameObject(new_object);
        }

        pending_duplicate_ = nullptr;
    }

    if (pending_reparent_object_) {
        if (pending_reparent_target_) {
            MindEngine::RemoveGameObject(pending_reparent_object_);
            pending_reparent_target_->AddChild(pending_reparent_object_);
        }
        else {
            MindEngine::RemoveGameObject(pending_reparent_object_);

            pending_reparent_object_->parent_.reset();

            MindEngine::AddGameObject(pending_reparent_object_);
        }

        pending_reparent_object_ = nullptr;
        pending_reparent_target_ = nullptr;
    }

    if (pending_reorder_object_ && reorder_target_) {
        auto parent = reorder_target_->parent_.lock();
        auto& list = parent ? parent->childrens_ : *MindEngine::GetAllGameObjects();

        list.erase(std::remove(list.begin(), list.end(), pending_reorder_object_), list.end());

        auto it = std::find(list.begin(), list.end(), reorder_target_);
        if (reorder_above_) {
            list.insert(it, pending_reorder_object_);
        }
        else {
            list.insert(it + 1, pending_reorder_object_);
        }

        pending_reorder_object_ = nullptr;
        reorder_target_ = nullptr;
    }
}

void HierarchyPanel::Render() {
    delete_selected_ = false;

    visible_flat_list_.clear();
    auto scene = MindEngine::GetAllGameObjects();

    BuildFlatList(*scene);

    BeginPanel();

    DrawDropZoneAbove(scene);

    for (const auto& object : *scene) {
        if (!object->HasParent()) {
            DrawGameObjectNode(object);
        }
    }

	HandlePendingActions();

    ImGui::End();
}

std::shared_ptr<GameObject> HierarchyPanel::GetSelectedObject() const
{
    return last_clicked_object_;

}
