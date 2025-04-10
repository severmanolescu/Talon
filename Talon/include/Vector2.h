#pragma once

struct Vector2 {
	float x = .0f;
	float y = .0f;

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

	Vector2 operator+=(const Vector2& other) const {
		return { x + other.x, y + other.y };
	}

	Vector2 operator-=(const Vector2& other) const {
		return { x - other.x, y - other.y };
	}

	Vector2 operator*=(const Vector2& other) const {
		return { x * other.x, y * other.y };
	}

	Vector2 operator/=(const Vector2& other) const {
		return { x / other.x, y / other.y };
	}

	template<typename T>
	Vector2 operator*(T scalar) const {
		return { x * static_cast<float>(scalar), y * static_cast<float>(scalar) };
	}

	template<typename T>
	Vector2 operator/(T scalar) const {
		return { x / static_cast<float>(scalar), y / static_cast<float>(scalar) };
	}
};
