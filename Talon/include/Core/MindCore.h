#pragma once

#define REGISTER_COMPONENT(TYPE) \
    static std::shared_ptr<MindCore> Create() { return std::make_shared<TYPE>(); }

#include "Vector2.h"
#include "UIUtils.h"
#include "ISerializable.h"
#include "IComponentLifecycle.h"

#include "string"
#include "imgui.h"

//TO-DO implement some reflection (RTTR) or something 

class GameObject;

/**
 * @class MindCore
 * @brief Base class for all engine components.
 *
 * This class acts as a core abstraction layer for components such as physics,
 * rendering, animation, and user-defined scripts. It supports lifecycle management
 * and serialization to JSON.
 *
 * All components should inherit from this class.
 */
class MindCore: public ISerializable, public IComponentLifecycle, public UIUtils{
public:
	/**
	 * @brief Pointer to the GameObject this component is attached to.
	 */
	GameObject* game_object_ = nullptr;

	/**
	 * @brief Indicates if the component is active in the scene.
	 */
	bool active_ = true;

	/**
	 * @brief Virtual destructor to allow safe polymorphic destruction.
	 */
	virtual ~MindCore() = default;

	/**
	 * @brief Called before rendering the component's UI in the inspector.
	 *
	 * Used to wrap component UI inside a formatted panel.
	 * @param component_name Name of the component shown in the UI panel.
	 */
	virtual void BeginDraw(std::string component_name);

	/**
	 * @brief Draws debug gizmos for the component.
	 *
	 * Override in derived components that need to draw visual debugging tools (e.g., collider bounds).
	 */
	virtual void DrawGizmo();

	/**
	 * @brief Factory method used for component registration and instantiation.
	 *
	 * Should be overridden using the REGISTER_COMPONENT macro.
	 * @return A shared pointer to a new instance of the component.
	 */
	virtual std::shared_ptr<MindCore> Create();
};
