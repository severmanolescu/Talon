#include "WindowManager.h"

#include <iostream>

void WindowManager::ResizeSceneTexture(int width, int height) {
	if (scene_texture_) SDL_DestroyTexture(scene_texture_);

	width_ = width;
	height_ = height;

	scene_texture_ = SDL_CreateTexture(
		renderer_,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width,
		height
	);

	if (!scene_texture_) {
		std::cerr << "[WindowManager] Scene texture creation failed: " << SDL_GetError() << "\n";
		return;
	}
}

void WindowManager::ResizeGameTexture(int width, int height) {
	if (game_texture_) SDL_DestroyTexture(scene_texture_);

	width_ = width;
	height_ = height;

	game_texture_ = SDL_CreateTexture(
		renderer_,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width,
		height
	);

	if (!game_texture_) {
		std::cerr << "[WindowManager] Scene texture creation failed: " << SDL_GetError() << "\n";
		return;
	}
}

bool WindowManager::Init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "[WindowManager] SDL_Init failed: " << SDL_GetError() << "\n";
		return false;
	}

	Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (!window_) {
		std::cerr << "[WindowManager] Window creation failed: " << SDL_GetError() << "\n";
		return false;
	}

	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer_) {
		std::cerr << "[WindowManager] Renderer creation failed: " << SDL_GetError() << "\n";
		return false;
	}

	scene_texture_ = SDL_CreateTexture(
		renderer_,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		800,
		600
	);


	return true;
}

void WindowManager::Clear() {
	SDL_SetRenderDrawColor(renderer_, 30, 30, 30, 255);
	SDL_RenderClear(renderer_);
}

void WindowManager::Present() {
	SDL_RenderPresent(renderer_);
}

void WindowManager::Shutdown() {
	if (renderer_) SDL_DestroyRenderer(renderer_);
	if (window_) SDL_DestroyWindow(window_);

	if (scene_texture_) {
		SDL_DestroyTexture(scene_texture_);
		scene_texture_ = nullptr;
	}

	if (game_texture_) {
		SDL_DestroyTexture(game_texture_);
		game_texture_ = nullptr;
	}

	SDL_Quit();
}
