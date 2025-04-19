#pragma once

#include <SDL2/SDL.h>

/**
 * @brief Static manager for SDL window and renderer operations.
 *
 * Handles window creation, scene rendering target, resizing, and cleanup.
 */
class WindowManager {
public:
    /// Current window width.
    static inline int width_ = 800;

    /// Current window height.
    static inline int height_ = 600;

    /**
     * @brief Initializes the SDL window and renderer.
     *
     * @param title Title of the window.
     * @param width Initial width.
     * @param height Initial height.
     * @return true if initialization succeeded, false otherwise.
     */
    static bool Init(const char* title, int width, int height);

    /**
     * @brief Clears the current rendering target.
     */
    static void Clear();

    /**
     * @brief Presents the rendered frame to the window.
     */
    static void Present();

    /**
     * @brief Cleans up SDL window, renderer, and texture resources.
     */
    static void Shutdown();

    /**
     * @brief Resizes the scene texture used for off-screen rendering.
     *
     * @param width New width.
     * @param height New height.
     */
    static void ResizeSceneTexture(int width, int height);

    /**
     * @brief Gets the SDL renderer.
     *
     * @return Pointer to SDL_Renderer.
     */
    static SDL_Renderer* GetRenderer() {
        return renderer_;
    }

    /**
     * @brief Gets the SDL window.
     *
     * @return Pointer to SDL_Window.
     */
    static SDL_Window* GetWindow() {
        return window_;
    }

    /**
     * @brief Gets the current scene render target texture.
     *
     * @return Pointer to SDL_Texture used for scene rendering.
     */
    static SDL_Texture* GetSceneTexture() {
        return scene_texture_;
    }

private:
    /// Pointer to SDL_Window.
    static inline SDL_Window* window_ = nullptr;

    /// Pointer to SDL_Renderer.
    static inline SDL_Renderer* renderer_ = nullptr;

    /// Texture used as off-screen render target for scene panel.
    static inline SDL_Texture* scene_texture_ = nullptr;
};
