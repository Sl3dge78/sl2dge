#pragma once

#include <math.h>

#include <SDL\SDL.h>

namespace sl2dge {

class Point {
public:
	int x, y;

public:
	Point() {
		x = 0;
		y = 0;
	}
	Point(int x, int y) :
			x(x), y(y) {}
	Point(const SDL_Rect &a) {
		x = a.x;
		y = a.y;
	}

	operator SDL_Point() const { return SDL_Point{ x, y }; }

	inline Point &operator=(const SDL_Point &a) {
		x = a.x;
		y = a.y;
		return *this;
	}

	friend inline bool operator==(const Point &lhs, const Point &rhs) {
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}

	Point &operator+=(const Point &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	friend Point operator+(Point lhs, const Point &rhs) {
		lhs += rhs;
		return lhs;
	}

	Point &operator*=(const int &rhs) {
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	friend Point operator*(Point lhs, const int &rhs) {
		lhs *= rhs;
		return lhs;
	}

	Point &operator/=(const int &rhs) {
		this->x /= rhs;
		this->y /= rhs;

		return *this;
	}

	friend Point operator/(Point lhs, const int &rhs) {
		lhs /= rhs;
		return lhs;
	}

	void Clamp(const Point &rhs) {
		if (this->x < rhs.x)
			this->x = rhs.x;
		if (this->y < rhs.y)
			this->y = rhs.y;
	}

	bool IsZero() { return (x == 0 && y == 0); }
};

// TODO MOVE THAT

} // namespace sl2dge
