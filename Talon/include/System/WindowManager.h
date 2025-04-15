#pragma once

#include <SDL2/SDL.h>

class WindowManager {
public:
	static inline int width_  = 800;
	static inline int height_ = 600;

	static bool Init(const char* title, int width, int height);

	static void Clear();

	static void Present();

	static void Shutdown();

	static void ResizeSceneTexture(int width, int height);

	static SDL_Renderer* GetRenderer() {
		return renderer_;
	}

	static SDL_Window* GetWindow() {
		return window_;
	}

	static SDL_Texture* GetSceneTexture() {
		return scene_texture_;
	}

private:
	static inline SDL_Window* window_ = nullptr;
	static inline SDL_Renderer* renderer_ = nullptr;

	static inline SDL_Texture* scene_texture_ = nullptr;
};
