#pragma once

#include "string"
#include "cmath"


/**
 * @struct Vector2
 * @brief Represents a 2D vector with floating point values.
 *
 * Provides basic vector arithmetic, normalization, magnitude,
 * distance calculation, and clamping functionality.
 */
struct Vector2 {
	float x = .0f;	///< X component of the vector.
	float y = .0f;	///< Y component of the vector.

	/// @brief Returns a vector with both components set to 0.
	static Vector2 Zero() {
		return Vector2{ .0f, .0f };
	}

	/// @brief Calculates the magnitude (length) of the vector.
	/// @return The magnitude as a float.
	float Magnitude() const {
		return std::sqrt(x * x + y * y);
	}

	/// @brief Converts the vector to a string format: (x, y)
	std::string ToString() const {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	/// @brief Normalizes the vector in-place (makes it unit length).
	void Normalize() {
		float magnitude = Magnitude();

		if (magnitude == 0) {
			x = 0;
			y = 0;

			return;
		}

		x /= magnitude;
		y /= magnitude;
	}

	/// @brief Returns a normalized copy of the vector.
	/// @return A new vector with magnitude 1 (or zero if input is zero).
	Vector2 Normalized() const {
		float magnitude = Magnitude();

		if (magnitude == 0) {
			return { 0, 0 };
		}

		return { x / magnitude, y / magnitude };
	}

	/// @brief Clamps the x and y values within a specified min and max.
	void Clamp(float min, float max) {
		x = std::fmax(min, std::fmin(x, max));
		y = std::fmax(min, std::fmin(y, max));
	}

	/// @brief Calculates the Euclidean distance to another vector.
	/// @param other The other vector.
	/// @return The distance as a float.
	float Distance(const Vector2& other) {
		float dx = x - other.x;
		float dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	/// @name Operator Overloads
	/// @{

	Vector2 operator+(const Vector2& other) const {
		return {x + other.x, y + other.y};
	}

	Vector2 operator-(const Vector2& other) const {
		return { x - other.x, y - other.y };
	}

	Vector2 operator*(const Vector2& other) const {
		return { x * other.x, y * other.y };
	}

	Vector2 operator/(const Vector2& other) const {
		return { x / other.x, y / other.y };
	}

	void operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
	}

	void operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
	}

	void operator*=(const Vector2& other) {
		x *= other.x;
		y *= other.y;
	}

	void operator/=(const Vector2& other) {
		x /= other.x;
		y /= other.y;
	}

	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vector2& other) const {
		return !(x == other.x && y == other.y);
	}

	Vector2& operator=(const Vector2& other) {
		x = other.x;
		y = other.y;
		return *this;
	}


	/// @brief Multiplies the vector by a scalar.
	template<typename T>
	Vector2 operator*(T scalar) const {
		return { x * static_cast<float>(scalar), y * static_cast<float>(scalar) };
	}

	/// @brief Multiplies the vector by a scalar in-place.
	template<typename T>
	void operator*=(T scalar) {
		x *= static_cast<float>(scalar);
		y *= static_cast<float>(scalar);
	}

	/// @brief Divides the vector by a scalar.
	template<typename T>
	Vector2 operator/(T scalar) const {
		return { x / static_cast<float>(scalar), y / static_cast<float>(scalar) };
	}

	/// @brief Divides the vector by a scalar in-place.
	template<typename T>
	void operator/=(T scalar) {
		x /= static_cast<float>(scalar);
		y /= static_cast<float>(scalar);
	}

	/// @}
};
