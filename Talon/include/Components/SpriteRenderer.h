#pragma once

#include <SDL2/SDL.h>

#include "MindCore.h"
#include "Transform.h"

#include "string"

#include "memory"

class SpriteRenderer : public MindCore {
public:
	int width_ = 16;
	int height_ = 16;

	Vector2 pivot_ = { 0.5f, 0.5f };

	SpriteRenderer(SDL_Renderer* renderer) : renderer_(renderer) {
		priority = 15;
	}

	void SetImage(const std::string& path);

	void SetTexture(SDL_Texture* texture);

	void SetSourceRect(const SDL_Rect &rect);

	void ClearSourceRect();

	void Awake() override;

	void Update() override;

	void DrawUI() override;

	void Render();
private:
	std::shared_ptr<Transform> transform_;

	SDL_Renderer* renderer_;

	SDL_Texture* texture_ = nullptr;

	SDL_Rect source_rect_{};
	bool use_source_rect_ = false;
};
