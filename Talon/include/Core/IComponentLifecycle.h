#pragma once

/**
 * @interface IComponentLifecycle
 * @brief Interface for defining component lifecycle methods.
 *
 * This interface defines the standard lifecycle hooks that any component
 * can implement to hook into the engine's object/component lifecycle.
 *
 * Classes inheriting from this can override:
 * - Awake: called when the component is created.
 * - Start: called before the first frame update.
 * - Update: called every frame.
 * - OnDestroy: called before the component is removed.
 */
class IComponentLifecycle {
public:
    /// Called once when the component is created.
    virtual void Awake() {}
    /// Called after Awake, before the first Update.
    virtual void Start() {}
    /// Called every frame during the game loop.
    virtual void Update() {}
    /// Called when the component is destroyed.
    virtual void OnDestroy() {}
};