#pragma once

#include "unordered_map"
#include <functional>
#include "memory"
#include "string"

class MindCore;

/// Alias for a function that creates a MindCore component.
using ComponentCreator = std::function<std::shared_ptr<MindCore>()>;

/**
 * @class ComponentFactory
 * @brief Singleton factory for dynamically creating components.
 *
 * Used during deserialization to instantiate components based on string names.
 * Components must register themselves using the `Register` method.
 *
 * Example:
 * ```cpp
 * ComponentFactory::Instance().Register("Transform", []() {
 *     return std::make_shared<Transform>();
 * });
 * ```
 *
 * Then:
 * ```cpp
 * auto component = ComponentFactory::Instance().Create("Transform");
 * ```
 */
class ComponentFactory {
public:
    /**
     * @brief Gets the singleton instance of the ComponentFactory.
     * @return A reference to the factory instance.
     */
    static ComponentFactory& Instance() {
        static ComponentFactory instance;
        return instance;
    }

    /**
     * @brief Registers a component creator function with a name.
     * @param name The string key (usually the component type name).
     * @param creator A lambda or function that returns a new instance of the component.
     */
    void Register(const std::string& name, ComponentCreator creator) {
        creators_[name] = creator;
        component_names_.push_back(name);
    }

    /**
     * @brief Creates a component by name if it's registered.
     * @param name The name of the registered component.
     * @return A shared pointer to the newly created MindCore component, or nullptr if not found.
     */
    std::shared_ptr<MindCore> Create(const std::string& name) {
        if (creators_.find(name) != creators_.end()) {
            return creators_[name]();
        }
        return nullptr;
    }

    /**
     * @brief Get all components name in the scene.
     *
     * @return A vector of string with all the available components.
     */
    const std::vector<std::string>& GetComponentNames() const {
        return component_names_;
    }

private:
    /// Internal map of registered creators.
    std::unordered_map<std::string, ComponentCreator> creators_;

    /**
     * @brief List of all root-level GameObjects in the scene
     * stored for the inspector.
     */
    std::vector<std::string> component_names_;
};