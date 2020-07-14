#include "Camera.h"

#include <SDL\SDL.h>

#include "Point.h"
#include "scene/Transform.h"

namespace sl2dge {

Camera::Camera(const int width, const int height, const float zoom) {
	this->width_ = width;
	this->height_ = height;
	this->zoom_ = zoom;
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

void CameraSystem::update() {
	for (auto &e : entities_) {
		auto cam = e->get_component<Camera>();
		auto transform = e->get_component<Transform>();
		cam->viewport_.w = int(float(cam->width_) / cam->zoom_);
		cam->viewport_.h = int(float(cam->height_) / cam->zoom_);
		cam->viewport_.x = int(transform->position.x) - (cam->viewport_.w / 2) + 8;
		cam->viewport_.y = int(transform->position.y) - (cam->viewport_.h / 2) + 8;
	};
} // namespace sl2dge

} // namespace sl2dge
