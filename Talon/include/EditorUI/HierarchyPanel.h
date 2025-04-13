#pragma once

#include "GameObject.h"

constexpr const char* hierarchy_gameobject_identifier = "HIERARCHY_GAMEOBJECT";
constexpr const char* new_game_object_name = "New GameObject";

constexpr const int drop_zone_height = 2;

class HierarchyPanel {
public:
	void Render();

private:
	std::shared_ptr<GameObject> renaming_target_ = nullptr;

	std::shared_ptr<GameObject> pending_duplicate_ = nullptr;

	std::shared_ptr<GameObject> pending_reparent_target_ = nullptr;
	std::shared_ptr<GameObject> pending_reparent_object_ = nullptr;

	std::shared_ptr<GameObject> pending_reorder_object_ = nullptr;
	std::shared_ptr<GameObject> reorder_target_ = nullptr;

	std::vector<std::shared_ptr<GameObject>> selected_objects_;
	std::shared_ptr<GameObject> last_clicked_object_ = nullptr;

	std::vector<std::shared_ptr<GameObject>> visible_flat_list_;

	bool reorder_above_ = true;

	bool delete_selected_ = false;

	char rename_buffer_[128] = "";

	char search_buffer_[128] = "";

	std::string GenerateUniqueName(const std::string& base_name, const std::shared_ptr<GameObject>& parent);

	void DrawGameObjectNode(const std::shared_ptr<GameObject>& object);

	bool NameMatchesSearch(const std::string& name, const char* search);

	void BuildFlatList(const std::vector<std::shared_ptr<GameObject>>& objects);

	void HandleUserKeyboardInput(const std::shared_ptr<GameObject>& object);

	void DrawPopup(const std::shared_ptr<GameObject>& object);

	void BeginPanel();

	void DrawDropZoneAbove(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>>);
	void DrawDropZoneBelow(std::shared_ptr<GameObject> object);

	void RenameObject(const std::shared_ptr<GameObject>& object);

	void HandlePendingActions();
};