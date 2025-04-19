#pragma once

/**
 * @enum EngineMode
 * @brief Enum to represent the current state of the engine.
 */
enum class EngineMode {
	Edit,	///< Editor mode - editing the scene, not playing it.
	Play    ///< Play mode - running the game logic.
};

/**
 * @class SceneManager
 * @brief Manages the state of the engine and handles scene persistence.
 *
 * Provides utilities to check the current engine mode (Edit/Play),
 * as well as to save and load the scene data.
 */
class SceneManager{
public:
	/**
	 * @brief Get the current mode of the engine.
	 * @return The current EngineMode (Edit or Play).
	 */
	static EngineMode GetMode() {
		return running_mode_;
	}

	/**
	 * @brief Check if the engine is currently running in Play mode.
	 * @return True if in Play mode, false otherwise.
	 */
	static bool IsPlaying() {
		return running_mode_ == EngineMode::Play;
	}

	/**
	 * @brief Set the current engine mode.
	 * @param mode The new engine mode (Edit or Play).
	 */
	static void SetRunningMode(EngineMode mode) {
		running_mode_ = mode;
	}

	/**
	 * @brief Save the current scene to disk.
	 *
	 * This will serialize all active GameObjects and their components.
	 */
	static void SaveScene();

	/**
	 * @brief Load a scene from disk.
	 *
	 * This will deserialize scene data and rebuild GameObjects and components.
	 */
	static void LoadScene();

private:
	/**
	 * @brief Tracks the current running mode of the engine.
	 */
	static EngineMode running_mode_;
};