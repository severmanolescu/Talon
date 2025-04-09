#pragma once

#include <SDL2/SDL.h>

class InputSystem {
public:
	static void Update() {
		SDL_PumpEvents();

		keyboard_state_ = SDL_GetKeyboardState(NULL);
	}

	static bool GetKey(SDL_Scancode key) {
		return keyboard_state_ && keyboard_state_[key];
	}

	static bool GetKeyDown(SDL_Scancode key) {
		return GetKey(key) && !last_state_[key];
	}

	static void LateUpdate() {
		memcpy(last_state_, keyboard_state_, SDL_NUM_SCANCODES);
	}

private:
	static inline const Uint8* keyboard_state_ = nullptr;
	static inline Uint8 last_state_[SDL_NUM_SCANCODES] = { 0 };

};