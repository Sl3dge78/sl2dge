#include "Transform.h"

namespace sl2dge {

Transform::Transform() {
	local_position_ = Vector2f(0, 0);
}

Transform::Transform(const Vector2f &position) {
	this->local_position_ = position;
}

Transform::Transform(const float x, const float y) {
	local_position_ = Vector2f(x, y);
}

Transform::~Transform() {
	remove_all_children();
}

Vector2i Transform::tiled_position() {
	return Vector2i(int(position().x) / 16, int(position().y) / 16);
}

Vector2f Transform::position() const {
	if (parent != nullptr) {
		return parent->position() + local_position_;
	} else {
		return local_position_;
	}
}

void Transform::translate(const Vector2f &translation) {
	local_position_ += translation;
}

void Transform::set_position(const Vector2f &pos) {
	local_position_ = pos;
}

void Transform::set_position(const float x, const float y) {
	local_position_.x = x;
	local_position_.y = y;
}

/* Parent / child */

std::vector<Transform *> Transform::get_children() const {
	return childs_;
}

void Transform::add_children(Transform *child) {
	childs_.push_back(child);
	child->parent = this;
}

void Transform::remove_children(Transform *child) {
	for (auto it = childs_.begin(); it != childs_.end(); ++it) {
		if (*it == child) {
			(*it)->parent = nullptr;
			childs_.erase(it);
			return;
		}
	}
}

void Transform::remove_all_children() {
	for (auto &e : childs_) {
		e->parent = nullptr;
	}
	childs_.clear();
}
void Transform::start(Game *game) {
}
void Transform::update(Game *game) {
}
void Transform::handle_events(Game *game, SDL_Event const &e) {
}
void Transform::draw(Game *game) {
}
void Transform::load(const pugi::xml_node &node) {
	this->local_position_.x = node.attribute("local_position_x").as_int();
	this->local_position_.y = node.attribute("local_position_y").as_int();
	this->z = node.attribute("z").as_int();
}
void Transform::save(pugi::xml_node &node) {
	node.append_attribute("local_position_x").set_value(local_position_.x);
	node.append_attribute("local_position_y").set_value(local_position_.y);
	node.append_attribute("z").set_value(z);
}
} // namespace sl2dge
