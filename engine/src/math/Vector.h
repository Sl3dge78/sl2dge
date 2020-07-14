#pragma once

#include <math.h>

#include <SDL\SDL.h>

class Vector2i;

class Vector2f {
public:
	float x;
	float y;

	Vector2f();
	Vector2f(const float x, const float y);
	;

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

	void normalize();

	float length();
};

class Vector2i {
public:
	int x;
	int y;

	Vector2i();
	Vector2i(const int x, const int y);
	;

	operator Vector2f() const { return Vector2f(float(x), float(y)); }

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

	float length();
};
