#include "Animator.h"
#include "GameObject.h"
#include "AnimatorStateMachine.h"

#include "iostream"

void Animator::Awake() {
	sprite_renderer_ = game_object_->GetComponent<SpriteRenderer>();
	animator_state_machine_ = game_object_->GetComponent<AnimatorStateMachine>();
}

void Animator::Start() {
    sprite_renderer_->SetTexture(spritesheet_);
}

void Animator::Update() {
	if (!sprite_renderer_ || frame_clips_.empty()) return;

	frame_timer_ += 0.016f;

	if (frame_timer_ >= frame_duration_) {
		frame_timer_ = 0.0f;

		if (frame_index_ >= frame_clips_.size()) {
			frame_index_ = 0;
		}

		sprite_renderer_->SetSourceRect(frame_clips_[frame_index_]);

        if (frame_events_.find(frame_index_) != frame_events_.end()) {
            for (auto& function : frame_events_[frame_index_]) {
                function();
            }
        }

		frame_index_++;
	}
}

void Animator::SetSpriteSheet(std::string path, SDL_Renderer* renderer) {
    renderer_ = renderer;
    frame_clips_.clear();
    frame_index_ = 0;

    if (spritesheet_) {
        SDL_DestroyTexture(spritesheet_);
        spritesheet_ = nullptr;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "[SpriteRenderer] Failed to load image: " << path << "\n";
        return;
    }

    spritesheet_ = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);

    if (!spritesheet_) {
        std::cerr << "[SpriteRenderer] Failed to create texture: " << path << "\n";
    }

    if (sprite_renderer_) {
        sprite_renderer_->SetTexture(spritesheet_);
    }

    for (int row = 0; row < rows_; row++) {
        for (int column = 0; column < columns_; column++) {
            SDL_Rect frame;

            frame.x = column * sprite_width_;
            frame.y = row * sprite_height_;

            frame.w = sprite_width_;
            frame.h = sprite_height_;

            frame_clips_.push_back(frame);
        }
    }
}