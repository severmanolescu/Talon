#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include "MindCore.h"
#include "SpriteRenderer.h"

#include "memory"
#include "vector"

#include <functional>
#include <unordered_map>

class AnimatorStateMachine;

class Animator : public MindCore {
public:
	short sprite_width_ = 16;
	short sprite_height_ = 16;

	short columns_ = 0;
	short rows_ = 0;

	std::unordered_map<int, std::vector<std::function<void()>>> frame_events_;

	void Awake() override;
	void Start() override;
	void Update() override;

	void SetSpriteSheet(std::string path, SDL_Renderer* renderer);

	SDL_Renderer* GetRender() {
		return renderer_;
	}
private:
	std::shared_ptr<AnimatorStateMachine> animator_state_machine_;

	std::shared_ptr<SpriteRenderer> sprite_renderer_;

	SDL_Renderer* renderer_ = nullptr;

	SDL_Texture* spritesheet_ = nullptr;

	std::vector<SDL_Rect> frame_clips_;

	int frame_index_ = 0;

	float frame_timer_ = 0.0f;
	float frame_duration_ = 0.15f;
};