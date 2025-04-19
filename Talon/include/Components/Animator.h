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

/// @todo Optimize frame update logic

/// @addtogroup Components
/// @{

/**
 * @brief Handles sprite animation using a spritesheet.
 *
 * The Animator component works together with the SpriteRenderer and optionally
 * an AnimatorStateMachine to control frame-by-frame animations.
 */
class Animator : public MindCore {
public:
	/// Width of a single frame in the spritesheet.
	int frame_width_ = 16;

	/// Height of a single frame in the spritesheet.=
	int frame_height_ = 16;

	/// Time in seconds each frame is displayed.
	float frame_duration_ = 0.15f;

	/// Number of columns in the spritesheet.
	int spritesheet_columns_ = 0;
	/// Number of rows in the spritesheet.
	int spritesheet_rows = 0;

	/// Constructor sets the component execution priority.
	Animator() {
		priority = 14;
	}

	/// Called during component setup.
	void Awake() override;
	/// Called every frame to update the animation.
	void Update() override;

	/**
	 * @brief Loads and slices the spritesheet into individual frames.
	 * @param path File path to the spritesheet.
	 * @param renderer SDL renderer used to load the texture.
	 */
	void SetSpriteSheet(std::string path, SDL_Renderer* renderer);

	/// Sets the internal SDL_Renderer.
	void SetRender(SDL_Renderer* renderer) {
		renderer_ = renderer;
	}

	/// Gets the current SDL_Renderer.
	SDL_Renderer* GetRender() {
		return renderer_;
	}

	/// Draws debug UI in the Inspector panel.
	void DrawUI() override;

	/// Serializes Animator state to JSON.
	void Serialize(nlohmann::json& json) override;
	/// Loads Animator state from JSON.
	void Deserialize(const nlohmann::json& json) override;

	REGISTER_COMPONENT(Animator)
private:
	/// Optional AnimatorStateMachine to manage transitions.
	std::shared_ptr<AnimatorStateMachine> animator_state_machine_;

	/// The SpriteRenderer to update frames on.
	std::shared_ptr<SpriteRenderer> sprite_renderer_;

	/// SDL_Renderer used to create textures.
	SDL_Renderer* renderer_ = nullptr;

	/// List of SDL_Rects defining individual frame clips.
	std::vector<SDL_Rect> frame_clips_;

	/// Current frame index being displayed.
	int frame_index_ = 0;

	/// Internal timer to track time between frames.
	float frame_timer_ = 0.0f;

	/// Generates SDL_Rect clips for each frame in the spritesheet.
	void GenerateFrameClips();

	/// Handles advancing frame index and updating the displayed frame.
	void UpdateFrames();
};

/// @}