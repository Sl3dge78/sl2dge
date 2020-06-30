#pragma once

#include <SDL\SDL.h>

#include "ECS/Component.h"
#include "Point.h"

namespace sl2dge {

class Camera : public ECS::Component {
public:
	Camera(const int width, const int height, const float zoom = 1, const Vector2 &position = { 0, 0 });
	~Camera(){};

	SDL_Point world_to_screen_transform(const SDL_Point &point) const;
	SDL_Rect world_to_screen_transform(const SDL_Rect &rect) const;
	SDL_Point screen_to_world_transform(const SDL_Point &point) const;

	// Sets the center of the viewport to pos
	void set_position(const Vector2 &pos);
	void translate(const Vector2 &translation);

	const SDL_Rect &viewport() const { return viewport_; }
	const Vector2 &center() const { return position_; }

	const float zoom() const { return zoom_; }
	void set_zoom(const float zoom) {
		this->zoom_ = zoom;
		this->viewport_ = { 0, 0, int(float(width_) / zoom), int(float(height_) / zoom) };
	}

private:
	SDL_Rect viewport_; // What the camera sees.
	Vector2 position_; // Position looking at.
	float zoom_;
	int width_, height_;
};
} // namespace sl2dge
