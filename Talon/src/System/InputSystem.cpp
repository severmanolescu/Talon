#include "InputSystem.h"

#include "fstream"
#include "json.hpp"

#include "iostream"

void InputSystem::Update() {
	SDL_PumpEvents();

	keyboard_state_ = SDL_GetKeyboardState(NULL);
}

bool InputSystem::GetKey(SDL_Scancode key) {
	if (!keyboard_state_ || key < 0 || key >= SDL_NUM_SCANCODES) return false;
	return keyboard_state_[key];
}

bool InputSystem::GetKey(const std::string& action) {
	if (bindings_.count(action) == 0) return false;
	return GetKey(bindings_[action]);
}

bool InputSystem::GetKeyDown(SDL_Scancode key) {
	if (!keyboard_state_ || key < 0 || key >= SDL_NUM_SCANCODES) return false;
	return keyboard_state_[key] && !last_state_[key];
}

bool InputSystem::GetKeyDown(const std::string& action) {
	if (bindings_.count(action) == 0) return false;
	return GetKeyDown(bindings_[action]);
}

bool InputSystem::GetKeyUp(SDL_Scancode key) {
	if (!keyboard_state_ || key < 0 || key >= SDL_NUM_SCANCODES) return false;
	return !keyboard_state_[key] && last_state_[key];
}

bool InputSystem::GetKeyUp(const std::string& action) {
	if (bindings_.count(action) == 0) return false;
	return GetKeyUp(bindings_[action]);
}

void InputSystem::LateUpdate() {
	if (keyboard_state_) {
		std::memcpy(last_state_, keyboard_state_, SDL_NUM_SCANCODES);
	}
}
void InputSystem::LoadFromJson(const std::string& path) {
	std::fstream input_file(path);

	if (!input_file.is_open()) {
		std::cerr << "Failed to open input file: " << path << std::endl;
		return;
	}

	nlohmann::json json_data;
	input_file >> json_data;

	for (auto& [action, key_name] : json_data.items()) {
		SDL_Scancode key = SDL_GetScancodeFromName(key_name.get<std::string>().c_str());
		if (key == SDL_SCANCODE_UNKNOWN) {
			std::cerr << "[InputSystem] Unknown key: " << key_name << std::endl;
			continue;
		}
		bindings_[action] = key;
	}
}