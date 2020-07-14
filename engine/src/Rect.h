#pragma once
#include "Point.h"
#include "math/Vector.h"
#include <SDL\SDL.h>

namespace sl2dge {

class Rect {
public:
	int x, y, w, h;

	Rect() {
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	Rect(int x, int y, int w, int h) :
			x(x), y(y), w(w), h(h) {}
	Rect(const SDL_Rect &a) {
		x = a.x;
		y = a.y;
		w = a.w;
		h = a.h;
	}

	operator SDL_Rect() const { return SDL_Rect{ x, y, w, h }; }

	Rect &operator=(const SDL_Rect &a) {
		x = a.x;
		y = a.y;
		w = a.w;
		h = a.h;
		return *this;
	}

	Rect &operator+=(const Rect &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->h += rhs.h;
		this->w += rhs.w;

		return *this;
	}

	Rect &operator+=(const Point &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
	}

	Rect &operator+=(const Vector2i &rhs) {
		this->x += int(rhs.x);
		this->y += int(rhs.y);
	}

	friend Rect operator+(Rect lhs, const Rect &rhs) {
		lhs += rhs;
		return lhs;
	}

	friend Rect operator+(Rect lhs, const Point &rhs) {
		lhs += rhs;
		return lhs;
	}

	friend Rect operator+(Rect lhs, const Vector2i &rhs) {
		lhs += rhs;
		return lhs;
	}

	Rect &operator*=(const int &rhs) {
		this->x *= rhs;
		this->y *= rhs;
		this->h *= rhs;
		this->w *= rhs;

		return *this;
	}

	friend Rect operator*(Rect lhs, const int &rhs) {
		lhs *= rhs;
		return lhs;
	}

	Rect &operator/=(const int &rhs) {
		this->x /= rhs;
		this->y /= rhs;
		this->h /= rhs;
		this->w /= rhs;

		return *this;
	}

	friend Rect operator/(Rect lhs, const int &rhs) {
		lhs /= rhs;
		return lhs;
	}

	Point ToPoint() {
		return Point(this->x, this->y);
	}

	void Clamp(const Rect &rhs) {
		if (this->x < rhs.x)
			this->x = rhs.x;
		if (this->y < rhs.y)
			this->y = rhs.y;
		if (this->w > rhs.w)
			this->w = rhs.w;
		if (this->h > rhs.h)
			this->h = rhs.h;
	}

	bool ContainsPartially(const Rect &b) {
		return x + w >= b.x && x <= b.w + b.x && y + h >= b.y && y <= b.h + b.y;
	}

	bool IsContainedPartially(const Rect &a) {
		return a.x + a.w >= x && a.x <= w + x && a.y + a.h >= y && a.y <= h + y;
	}

	bool Contains(const Rect &b) {
		return x <= b.x && x + w >= b.x + b.w && y <= b.y && y + h >= b.y + b.h;
	}

	bool IsContained(const Rect &a) {
		return a.x <= x && a.x + a.w >= x + w && a.y <= y && a.y + a.h >= y + h;
	}
};
} // namespace sl2dge
