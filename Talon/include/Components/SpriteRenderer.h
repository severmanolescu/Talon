#pragma once

#include <SDL2/SDL.h>

#include "WindowManager.h"
#include "MindCore.h"
#include "Transform.h"

#include "string"

#include "memory"

/// @addtogroup Components
/// @{

/**
 * @class SpriteRenderer
 * @brief Renders a 2D sprite using an image texture.
 *
 * Responsible for drawing textured quads (sprites) using SDL2. Can optionally render only a portion of the image using a source rectangle.
 *
 * @note Requires a valid SDL_Renderer. Automatically fetched from WindowManager on construction.
 *
 */
class SpriteRenderer : public MindCore {
public:
	/// Width of the rendered sprite.
	int width_ = 16;
	/// Height of the rendered sprite.
	int height_ = 16;

	/// Pivot point (0–1 range) for rendering (e.g., center is {0.5, 0.5}).
	Vector2 pivot_ = { 0.5f, 0.5f };

	/**
	 * @brief Default constructor. Initializes priority and fetches the default SDL_Renderer.
	 */
	SpriteRenderer() {
		priority = 15;

		renderer_ = WindowManager::GetRenderer();
	}

	/**
	 * @brief Constructor with a custom renderer.
	 * @param renderer A pointer to the SDL renderer to use.
	 */
	SpriteRenderer(SDL_Renderer* renderer) : renderer_(renderer) {
		priority = 15;
	}

	/**
	 * @brief Loads an image and sets it as the current texture.
	 * @param path The path to the image file.
	 */
	void SetImage(const std::string& path);

	/**
	 * @brief Sets the texture manually.
	 * @param texture The SDL texture to use for rendering.
	 */
	void SetTexture(SDL_Texture* texture);

	/**
	 * @brief Specifies the region of the texture to render.
	 * @param rect SDL_Rect representing the source region.
	 */
	void SetSourceRect(const SDL_Rect &rect);

	/**
	 * @brief Clears any previously set source rectangle (renders full texture).
	 */
	void ClearSourceRect();

	/**
	 * @brief Called during the Awake lifecycle to prepare resources.
	 */
	void Awake() override;

	/**
	 * @brief Called every frame to update logic.
	 */
	void Update() override;

	/**
	 * @brief Renders editable values to the UI (Inspector).
	 */
	void DrawUI() override;

	/**
	 * @brief Draws the sprite to the screen.
	 */
	void Render();

	/**
	 * @brief Serializes the sprite renderer's state to JSON.
	 * @param json JSON object to populate.
	 */
	void Serialize(nlohmann::json& json) override;

	/**
	 * @brief Deserializes the sprite renderer's state from JSON.
	 * @param json JSON object to read from.
	 */
	void Deserialize(const nlohmann::json& json) override;

	/// Returns the name of the component type.
	const char* GetTypeName() const override { return "SpriteRenderer"; }

	REGISTER_COMPONENT(SpriteRenderer)
private:
	/// SDL renderer used for drawing.
	SDL_Renderer* renderer_;

	/// Texture used for the sprite.
	SDL_Texture* texture_ = nullptr;

	/// Rectangle defining a sub-region of the texture to render.
	SDL_Rect source_rect_{};
	/// If true, source_rect_ is used for rendering.
	bool use_source_rect_ = false;

	/// Path to the texture file (for serialization).
	std::string image_path;
};

/// @}
