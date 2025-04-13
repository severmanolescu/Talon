#pragma once

#include <SDL2/SDL.h>

class WindowManager {
public:
	bool Init(const char* title, int width, int height);

	void Clear();

	void Present();

	void Shutdown();

	SDL_Renderer* GetRenderer() const;

	SDL_Window* GetWindow() const;

private:
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
};
