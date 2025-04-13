#include "WindowManager.h"

#include <iostream>

bool WindowManager::Init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
		return false;
	}

	window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!window_) {
		std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
		return false;
	}

	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer_) {
		std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
		return false;
	}

	return true;
}

void WindowManager::Clear() {
	SDL_SetRenderDrawColor(renderer_, 30, 30, 30, 255); // dark gray
	SDL_RenderClear(renderer_);
}

void WindowManager::Present() {
	SDL_RenderPresent(renderer_);
}

void WindowManager::Shutdown() {
	if (renderer_) SDL_DestroyRenderer(renderer_);
	if (window_) SDL_DestroyWindow(window_);
	SDL_Quit();
}

SDL_Renderer* WindowManager::GetRenderer() const {
	return renderer_;
}

SDL_Window* WindowManager::GetWindow() const
{
	return window_;
}
