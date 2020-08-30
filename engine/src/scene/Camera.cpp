#include "Camera.h"

#include <SDL/SDL.h>

#include "scene/Transform.h"

namespace sl2dge {
Camera *Camera::main_camera = nullptr;

Camera::Camera(const int width, const int height, const float zoom) {
	this->width_ = width;
	this->height_ = height;
	this->zoom_ = zoom;

	Camera::main_camera = this;
}

void Camera::load(pugi::xml_node const &node) {
	this->width_ = node.attribute("width").as_int();
	this->height_ = node.attribute("height").as_int();
	this->zoom_ = node.attribute("zoom").as_float();

	Camera::main_camera = this;
}

void Camera::save(pugi::xml_node &node) {
	node.append_attribute("width").set_value(this->width_);
	node.append_attribute("height").set_value(this->height_);
	node.append_attribute("zoom").set_value(this->zoom_);
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

void Camera::update(Game *game) {
	auto transform = entity()->get_component<Transform>();
	viewport_.w = int(float(width_) / zoom_);
	viewport_.h = int(float(height_) / zoom_);
	viewport_.x = int(transform->position().x) - (viewport_.w / 2) + 8;
	viewport_.y = int(transform->position().y) - (viewport_.h / 2) + 8;
}

} // namespace sl2dge
