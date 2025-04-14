#pragma once

#include <SDL2/SDL.h>

#include "MindCore.h"
#include "Transform.h"

#include "string"

#include "memory"

class SpriteRenderer : public MindCore {
public:
	int width_ = 32;
	int height_ = 64;

	SpriteRenderer(SDL_Renderer* renderer) : renderer_(renderer) {}

	void SetImage(const std::string& path);

	void SetTexture(SDL_Texture* texture);

	void SetSourceRect(const SDL_Rect &rect);

	void ClearSourceRect();

	void Awake() override;

	void Update() override;

	void DrawUI() override;
private:
	std::shared_ptr<Transform> transform_;

	SDL_Renderer* renderer_;

	SDL_Texture* texture_ = nullptr;

	SDL_Rect source_rect_{};
	bool use_source_rect_ = false;
};
