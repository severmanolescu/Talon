#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>

/**
 * @brief Static input system for querying keyboard states using SDL.
 *
 * Supports key state queries (pressed, just pressed, released) and string-based bindings.
 */
class InputSystem {
public:

    /**
     * @brief Updates the current keyboard state. Must be called every frame.
     */
    static void Update();

    /**
     * @brief Checks if the key is currently held down.
     *
     * @param key SDL scan code of the key.
     * @return true if key is down, false otherwise.
     */
    static bool GetKey(SDL_Scancode key);

    /**
     * @brief Checks if a named key is currently held down.
     *
     * @param key String name of the key, based on loaded bindings.
     * @return true if key is down, false otherwise.
     */
    static bool GetKey(const std::string& key);

    /**
     * @brief Checks if the key was just pressed this frame.
     *
     * @param key SDL scan code of the key.
     * @return true if the key was just pressed.
     */
    static bool GetKeyDown(SDL_Scancode key);

    /**
     * @brief Checks if the named key was just pressed this frame.
     *
     * @param key String name of the key.
     * @return true if the key was just pressed.
     */
    static bool GetKeyDown(const std::string& key);

    /**
     * @brief Checks if the key was released this frame.
     *
     * @param key SDL scan code of the key.
     * @return true if the key was just released.
     */
    static bool GetKeyUp(SDL_Scancode key);

    /**
     * @brief Checks if the named key was released this frame.
     *
     * @param key String name of the key.
     * @return true if the key was just released.
     */
    static bool GetKeyUp(const std::string& key);

    /**
     * @brief Called after all input has been handled to save previous state.
     */
    static void LateUpdate();

    /**
     * @brief Loads key bindings from a JSON config file.
     *
     * The file should contain a mapping of string keys to SDL scancodes.
     *
     * @param path Path to JSON bindings file.
     */
    static void LoadFromJson(const std::string& path);

private:
    /// SDL's current key state array
    static inline const Uint8* keyboard_state_ = nullptr;

    /// Previous frame's key state array
    static inline Uint8 last_state_[SDL_NUM_SCANCODES] = { 0 };

    /// Mapping of string keys to SDL_Scancode values
    static inline std::unordered_map<std::string, SDL_Scancode> bindings_;
};
