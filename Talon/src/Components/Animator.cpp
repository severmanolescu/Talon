#include "Animator.h"
#include "GameObject.h"
#include "AnimatorStateMachine.h"

#include "iostream"

void Animator::Awake() {
	sprite_renderer_ = game_object_->GetComponent<SpriteRenderer>();
	animator_state_machine_ = game_object_->GetComponent<AnimatorStateMachine>();

    renderer_ = WindowManager::GetRenderer();
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

    for (int row = 0; row < spritesheet_rows; row++) {
        for (int column = 0; column < spritesheet_columns_; column++) {
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

void Animator::DrawUI(){
	BeginDraw("Animator");

    DrawInt2Control(
        "Frame",
        "Width",
        &frame_width_, 
        "Heihgt",
        &frame_height_
    );

    DrawInt2Control(
        "Spritehseet",
        "Columns",
        &spritesheet_columns_,
        "Rows",
        &spritesheet_rows
    );

    DrawFloatControl("Frame Duration", &frame_duration_);
    
	EndDraw();
}

void Animator::Serialize(nlohmann::json& json){
    nlohmann::json animator;

    animator["type"] = "Animator";

    animator["data"]["frame"] =
    {
        {"width", frame_width_},
        {"height", frame_height_},
        {"duration", frame_duration_}
    };

    animator["data"]["spritesheet"] =
    {
        {"columns", spritesheet_columns_},
        {"rows", spritesheet_rows}
    };

    animator["active"] = active_;

    json.push_back(animator);
}

void Animator::Deserialize(const nlohmann::json& json){
    if (json.contains("frame"))
    {
        frame_width_ = json["frame"]["width"];
        frame_height_ = json["frame"]["height"];
        frame_duration_ = json["frame"]["duration"];
    }

    if (json.contains("spritesheet"))
    {
        spritesheet_columns_ = json["spritesheet"]["columns"];
        spritesheet_rows = json["spritesheet"]["rows"];
    }
}
