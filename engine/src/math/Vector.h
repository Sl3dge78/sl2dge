#pragma once

#include <math.h>

#include <SDL\SDL.h>

class Vector2i;

class Vector2f {
public:
	float x;
	float y;

	Vector2f() {
		x = 0;
		y = 0;
	}

	Vector2f(const float x, const float y) :
			x(x), y(y) {}

	void normalize() {
		auto l = length();
		if (l == 0 || l == 1)
			return;
		x = x / l;
		y = y / l;
	}

	float length() {
		return float(sqrt(x * x + y * y));
	}
	bool is_zero() {
		return x == 0 && y == 0;
	}

	operator Vector2i() const;
	Vector2f &operator+=(const Vector2f &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	friend Vector2f operator+(Vector2f lhs, const Vector2f &rhs) {
		lhs += rhs;
		return lhs;
	}
	Vector2f &operator*=(const float &rhs) {
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	friend Vector2f operator*(Vector2f lhs, const float &rhs) {
		lhs *= rhs;
		return lhs;
	}
	friend bool operator==(const Vector2f &lhs, const Vector2f &rhs) {
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}
};

class Vector2i {
public:
	int x;
	int y;

	Vector2i() {
		x = 0;
		y = 0;
	}

	Vector2i(const int x, const int y) :
			x(x), y(y) {}

	float length() {
		return float(sqrt(x * x + y * y));
	}
	bool is_zero() {
		return x == 0 && y == 0;
	}

	operator Vector2f() const;
	Vector2i &operator+=(const Vector2i &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	friend Vector2i operator+(Vector2i lhs, const Vector2i &rhs) {
		lhs += rhs;
		return lhs;
	}
	Vector2i &operator*=(const int &rhs) {
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	friend Vector2i operator*(Vector2i lhs, const int &rhs) {
		lhs *= rhs;
		return lhs;
	}
	friend bool operator==(const Vector2i &lhs, const Vector2i &rhs) {
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}
};

inline Vector2f::operator Vector2i() const {
	return Vector2i(int(x), int(y));
}

inline Vector2i::operator Vector2f() const {
	return Vector2f(float(x), float(y));
}
