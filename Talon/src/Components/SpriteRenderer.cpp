#include <SDL2/SDL_image.h>

#include "SpriteRenderer.h"
#include "GameObject.h"

#include "iostream"

void SpriteRenderer::SetImage(const std::string& path) {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "[SpriteRenderer] Failed to load image: " << path << "\n";
        return;
    }

    texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);

    if (!texture_) {
        std::cerr << "[SpriteRenderer] Failed to create texture: " << path << "\n";
    }
}

void SpriteRenderer::SetSourceRect(const SDL_Rect& rect) {
    source_rect_ = rect;

    use_source_rect_ = true;
}

void SpriteRenderer::SetTexture(SDL_Texture* texture) {
    texture_ = texture;
}

void SpriteRenderer::ClearSourceRect() {
    use_source_rect_ = false;
}

void SpriteRenderer::Awake(){
    ui_frame_height_ = 4;

	transform_ = game_object_->GetTransform();
}

void SpriteRenderer::Update() {
	if (!transform_ || !renderer_) {
		return;
	}

	SDL_Rect dest;

    dest.x = static_cast<int>(transform_->position_.x);
    dest.y = static_cast<int>(transform_->position_.y);

    dest.w = width_;
    dest.h = height_;

    if (use_source_rect_) {
        SDL_RenderCopy(renderer_, texture_, &source_rect_, &dest);
    }
    else {
        SDL_RenderCopy(renderer_, texture_, nullptr, &dest);
    }
}

void SpriteRenderer::DrawUI(){
    BeginDraw("SpriteRenderer");

    DrawInt2Control(
        "Dimension",
        "Width",
        &width_,
        "Heihgt",
        &height_
    );

    EndDraw();
}
