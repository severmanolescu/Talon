#pragma once

#include <SDL2/SDL.h>

#include "string"
#include "unordered_map"

class InputSystem {
public:
	static void Update();

	static bool GetKey(SDL_Scancode key);
	static bool GetKey(const std::string& key);

	static bool GetKeyDown(SDL_Scancode key);
	static bool GetKeyDown(const std::string& key);

	static bool GetKeyUp(SDL_Scancode key);
	static bool GetKeyUp(const std::string& key);

	static void LateUpdate();

	static void LoadFromJson(const std::string& path);

private:
	static inline const Uint8* keyboard_state_ = nullptr;
	static inline Uint8 last_state_[SDL_NUM_SCANCODES] = { 0 };

	static inline std::unordered_map<std::string, SDL_Scancode> bindings_;
};
