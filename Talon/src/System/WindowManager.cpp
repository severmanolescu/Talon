#include "WindowManager.h"

#include <iostream>

bool WindowManager::Init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
		return false;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
		return false;
	}

	return true;
}

void WindowManager::Clear() {
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // dark gray
	SDL_RenderClear(renderer);
}

void WindowManager::Present() {
	SDL_RenderPresent(renderer);
}

void WindowManager::Shutdown() {
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Renderer* WindowManager::GetRenderer() const {
	return renderer;
}
