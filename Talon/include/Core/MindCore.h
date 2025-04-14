#pragma once

#include "Vector2.h"

#include "string"
#include "imgui.h"

//TO-DO implement some reflection (RTTR) or something 

class GameObject;

class MindCore {
public:
	GameObject* game_object_ = nullptr;

	bool active_ = true;

	float ui_frame_height_ = 5.0f;

	virtual void Awake();
	virtual void Start();

	virtual void Update();

	virtual void OnDestroy();

	virtual ~MindCore() = default;

	virtual void DrawUI();

	virtual void BeginDraw(std::string component_name);
	virtual void EndDraw();

	virtual void DrawVector2Control(
		const std::string& label,
		Vector2& values,
		float resetValue = 0.0f,
		float columnWidth = 100.0f
	);

	void DrawInt2Control(
		const std::string& label,
		const std::string& first_value_label,
		int* first_value,
		const std::string& second_value_label,
		int* second_value,
		float columnWidth = 100.0f
	);

	void DrawCheckbox(const std::string& label, bool* value);

	void DrawIntControl(const std::string& label, int* value);
	void DrawFloatControl(const std::string& label, float* value);

};
