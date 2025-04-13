#include "Animator.h"
#include "GameObject.h"
#include "AnimatorStateMachine.h"

#include "iostream"

void Animator::Awake() {
	sprite_renderer_ = game_object_->GetComponent<SpriteRenderer>();
	animator_state_machine_ = game_object_->GetComponent<AnimatorStateMachine>();
}

void Animator::UpdateFrames(){
    if (!sprite_renderer_ || frame_clips_.empty()) return;

    frame_timer_ += 0.016f;

    if (frame_timer_ >= frame_duration_) {
        frame_timer_ -= frame_duration_;

        if (frame_index_ < frame_clips_.size()) {
            sprite_renderer_->SetSourceRect(frame_clips_[frame_index_]);
        }

        frame_index_++;

        if (frame_index_ >= frame_clips_.size()) {
            frame_index_ = 0;
        }
    }
}

void Animator::Update() {
    UpdateFrames();
}

void Animator::GenerateFrameClips() {
    frame_clips_.clear();

    for (int row = 0; row < rows_; row++) {
        for (int column = 0; column < columns_; column++) {
            SDL_Rect frame;

            frame.x = column * frame_width_;
            frame.y = row * frame_height_;

            frame.w = frame_width_;
            frame.h = frame_height_;

            frame_clips_.push_back(frame);
        }
    }
}

void Animator::SetSpriteSheet(std::string path, SDL_Renderer* renderer) {
    if (path.empty() || !renderer) return;

    renderer_ = renderer;

    frame_index_ = 0;
    frame_timer_ = 0.0f;

    if (sprite_renderer_) {
        // For now until the camera is integrated
		sprite_renderer_->width_ = frame_width_ * 2;
		sprite_renderer_->height_ = frame_height_ * 2;

	    sprite_renderer_->SetImage(path);
    }

    GenerateFrameClips();

    UpdateFrames();
}