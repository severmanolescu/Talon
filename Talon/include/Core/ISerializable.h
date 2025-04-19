#pragma once

#include "json.hpp"

/**
 * @interface ISerializable
 * @brief Interface for serialization and deserialization of components or objects.
 *
 * Classes implementing this interface should define how to convert their data
 * to and from JSON using the nlohmann::json library. This is useful for saving
 * and loading scenes, components, and other data-driven elements.
 */
class ISerializable {
public:
	/**
	 * @brief Serializes the object's data to a JSON object.
	 * @param json The JSON object to write the serialized data into.
	 */
	virtual void Serialize(nlohmann::json& json) {};

	/**
	 * @brief Deserializes the object's data from a JSON object.
	 * @param json The JSON object to read the serialized data from.
	 */
	virtual void Deserialize(const nlohmann::json& json) {};
};