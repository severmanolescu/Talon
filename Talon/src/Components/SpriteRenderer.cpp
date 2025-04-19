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

    image_path = path;
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
    ui_frame_height_ = 6;
}

void SpriteRenderer::Render(){
    if (!renderer_) {
     return;
    }

    SDL_Rect dest;

    Vector2 relative_position = game_object_->GetTransform()->GetWorldPosition();

    dest.x = static_cast<int>(pivot_.x + relative_position.x);
    dest.y = static_cast<int>(pivot_.y + relative_position.y);

    dest.w = static_cast<int>(width_ * game_object_->GetTransform()->scale_.x);
    dest.h = static_cast<int>(height_ * game_object_->GetTransform()->scale_.y);

    // TO-DO: implement pivot
    //SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);

    //SDL_RenderDrawPoint(renderer_, dest.x, dest.y);
    //SDL_RenderDrawPoint(renderer_, dest.x - 1, dest.y);
    //SDL_RenderDrawPoint(renderer_, dest.x + 1, dest.y);
    //SDL_RenderDrawPoint(renderer_, dest.x, dest.y - 1);
    //SDL_RenderDrawPoint(renderer_, dest.x, dest.y + 1);

    if (use_source_rect_) {
        SDL_RenderCopy(renderer_, texture_, &source_rect_, &dest);
    }
    else {
        SDL_RenderCopy(renderer_, texture_, nullptr, &dest);
    }
}

void SpriteRenderer::Update() {
    Render();
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

	DrawVector2Control("Pivot", pivot_);

    EndDraw();
}
void SpriteRenderer::Serialize(nlohmann::json& json) {
    nlohmann::json sprite_renderer;

    sprite_renderer["type"] = "SpriteRenderer";

    sprite_renderer["data"]["sprite"] =
    {
        {"width", width_},
        {"height", height_}
    };

    sprite_renderer["data"]["pivot"] =
    {
        {"x", pivot_.x},
        {"y", pivot_.y}
    };

    sprite_renderer["data"]["image"] = image_path;

    sprite_renderer["active"] = active_;

    json.push_back(sprite_renderer);
}

void SpriteRenderer::Deserialize(const nlohmann::json& json) {
    if (json.contains("sprite"))
    {
        width_ = json["sprite"]["width"];
        height_ = json["sprite"]["height"];
    }

    if (json.contains("pivot"))
    {
        pivot_ =
        {
            json["pivot"]["x"],
            json["pivot"]["y"]
        };
    }

    if (json.contains("image")) {
        SetImage(json["image"]);
    }
}
