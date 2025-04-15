#pragma once

enum class EngineMode {
	Edit,
	Play
};

class SceneManager{
public:
	static EngineMode GetMode() {
		return running_mode_;
	}
	static bool IsPlaying() {
		return running_mode_ == EngineMode::Play;
	}

	static void SetRunningMode(EngineMode mode) {
		running_mode_ = mode;
	}

private:
	static EngineMode running_mode_;
};