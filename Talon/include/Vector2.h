#pragma once

#include "cmath"

struct Vector2 {
	float x = .0f;
	float y = .0f;

	static Vector2 Zero() {
		return Vector2{ .0f, .0f };
	}

	float Magnitude() const {
		return std::sqrt(x * x + y * y);
	}

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

	Vector2 Normalized() const {
		float magnitude = Magnitude();

		if (magnitude == 0) {
			return { 0, 0 };
		}

		return { x / magnitude, y / magnitude };
	}

	void Clamp(float min, float max) {
		x = std::fmax(min, std::fmin(x, max));
		y = std::fmax(min, std::fmin(y, max));
	}

	float Distance(const Vector2& other) {
		float dx = x - other.x;
		float dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

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

	template<typename T>
	Vector2 operator*(T scalar) const {
		return { x * static_cast<float>(scalar), y * static_cast<float>(scalar) };
	}

	template<typename T>
	void operator*=(T scalar) {
		x *= static_cast<float>(scalar);
		y *= static_cast<float>(scalar);
	}

	template<typename T>
	Vector2 operator/(T scalar) const {
		return { x / static_cast<float>(scalar), y / static_cast<float>(scalar) };
	}

	template<typename T>
	void operator/=(T scalar) {
		x /= static_cast<float>(scalar);
		y /= static_cast<float>(scalar);
	}
};
