#pragma once
#include <Vector2.h>
#include <string>

/**
 * @class UIUtils
 * @brief Utility base class for rendering custom ImGui UI elements for components.
 *
 * This class provides helper methods to render styled ImGui elements like
 * Vector2 controls, checkboxes, and numeric fields with labels.
 * It is inherited by component classes like Transform, Rigidbody, etc.,
 * to simplify UI code.
 */
class UIUtils {
public:
	/**
	 * @brief Padding height for consistent UI spacing.
	 */
	float ui_frame_height_ = 5.0f;

	/**
	 * @brief Called after drawing UI elements (e.g., to close containers or apply final styles).
	 */
	virtual void EndDraw();

	/**
	 * @brief Virtual method to draw the component-specific UI (to override in components).
	 */
	virtual void DrawUI() {};

	/**
	 * @brief Draws two float input fields representing a Vector2.
	 * @param label Label for the row.
	 * @param values Vector2 reference to bind.
	 * @param resetValue Default reset value.
	 * @param columnWidth Width reserved for the label.
	 */
	virtual void DrawVector2Control(
		const std::string& label,
		Vector2& values,
		float resetValue = 0.0f,
		float columnWidth = 100.0f
	);

	/**
	 * @brief Draws two integer input fields with custom labels.
	 * @param label Main label.
	 * @param first_value_label Label for the first input.
	 * @param first_value Pointer to first integer.
	 * @param second_value_label Label for the second input.
	 * @param second_value Pointer to second integer.
	 * @param columnWidth Width reserved for the label.
	 */
	void DrawInt2Control(
		const std::string& label,
		const std::string& first_value_label,
		int* first_value,
		const std::string& second_value_label,
		int* second_value,
		float columnWidth = 100.0f
	);

	/**
	 * @brief Draws a checkbox with a label.
	 * @param label Label for the checkbox.
	 * @param value Boolean pointer to bind.
	 */
	void DrawCheckbox(const std::string& label, bool* value);

	/**
	 * @brief Draws a single integer input with a label.
	 * @param label Label for the input.
	 * @param value Pointer to the integer value.
	 */
	void DrawIntControl(const std::string& label, int* value);

	/**
	 * @brief Draws a single float input with a label.
	 * @param label Label for the input.
	 * @param value Pointer to the float value.
	 */
	void DrawFloatControl(const std::string& label, float* value);

	/**
	 * @brief Returns the priority of the component using this utility.
	 * @return Execution priority.
	 */
	int GetExecutionPriority() const;
private:
	/**
	 * @brief Execution priority used for update ordering (lower means earlier).
	 */
	int priority = 100;

	friend class Transform;
	friend class BoxCollider;
	friend class Rigidbody;
	friend class SpriteRenderer;
	friend class Animator;
	friend class AnimatorStateMachine;
};