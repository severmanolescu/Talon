#pragma once

#include <SDL2/SDL.h>

inline bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
	return SDL_HasIntersection(&a, &b);
}
