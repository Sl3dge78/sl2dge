#include "Camera.h"

#include <SDL\SDL.h>

#include "Point.h"

namespace sl2dge {

Camera::Camera(const int width, const int height, const float zoom, const Vector2 &position) {
	this->width_ = width;
	this->height_ = height;
	this->set_zoom(zoom);
	set_position(position);
}

void Camera::set_position(const Vector2 &pos) {
	position_ = pos;
	viewport_.x = int(pos.x) - (viewport_.w / 2) + 8;
	viewport_.y = int(pos.y) - (viewport_.h / 2) + 8;
}

void Camera::translate(const Vector2 &translation) {
	set_position(position_ + translation);
}

SDL_Point Camera::world_to_screen_transform(const SDL_Point &point) const {
	return SDL_Point{ int(float(point.x - viewport_.x) * zoom_), int(float(point.y - viewport_.y) * zoom_) };
}

SDL_Rect Camera::world_to_screen_transform(const SDL_Rect &rect) const {
	return SDL_Rect{ int(float(rect.x - viewport_.x) * zoom_), int(float(rect.y - viewport_.y) * zoom_), int(float(rect.w) * zoom_), int(float(rect.h) * zoom_) };
}

SDL_Point Camera::screen_to_world_transform(const SDL_Point &point) const {
	return SDL_Point{ int(float(point.x / zoom_ + viewport_.x)), int(float(point.y / zoom_ + viewport_.y)) };
}

} // namespace sl2dge
