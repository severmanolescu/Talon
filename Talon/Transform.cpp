#include "Transform.h"
#include "GameObject.h"

Vector2 Transform::GetWorldPosition() {
	if (auto parent = game_object_->parent_.lock()) {
		return position_ + parent->GetTransform()->GetWorldPosition();
	}

	return position_;
}

void Transform::SetWorldPosition(const Vector2& world_pos) {
    if (auto parent = game_object_->parent_.lock()) {
        auto parent_world = parent->GetTransform()->GetWorldPosition();
        position_ = world_pos - parent_world;
    }
    else {
        position_ = world_pos;
    }
}