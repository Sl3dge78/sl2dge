#include "Vector.h"

Vector2f::Vector2f() {
	x = 0;
	y = 0;
}

Vector2f::Vector2f(const float x, const float y) :
		x(x), y(y) {}

inline Vector2f::operator Vector2i() const {
	return Vector2i(int(x), int(y));
}

inline void Vector2f::normalize() {
	auto l = length();
	if (l == 0 || l == 1)
		return;
	x = x / l;
	y = y / l;
}

inline float Vector2f::length() {
	return float(sqrt(x * x + y * y));
}

Vector2i::Vector2i() {
	x = 0;
	y = 0;
}

Vector2i::Vector2i(const int x, const int y) :
		x(x), y(y) {}

inline float Vector2i::length() {
	return float(sqrt(x * x + y * y));
}
