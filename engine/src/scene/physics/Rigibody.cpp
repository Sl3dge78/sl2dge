#include "Rigibody.h"

void sl2dge::Rigidbody::update(Game *game) {
	if (!speed.is_zero()) {
		auto translation = move_and_slide();
		entity()->transform()->translate(translation);
		speed = Vector2f(0, 0);
	}
}

Vector2f sl2dge::Rigidbody::move_and_slide() {
	const auto position = entity()->transform()->position();
	Vector2f ret = Vector2f(0, 0);

	// Move in X
	auto new_pos_x = position.x + speed.x;
	if (new_pos_x > 0 &&
			!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x), int(position.y + collider.y) }) &&
			!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x + collider.w), int(position.y + collider.y) }) &&
			!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x), int(position.y + collider.y + collider.h) }) &&
			!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x + collider.w), int(position.y + collider.y + collider.h) })) {
		ret.x = speed.x;
	}

	// Move in Y
	auto new_pos_y = position.y + speed.y;
	if (new_pos_y > 0 &&
			!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x), int(new_pos_y + collider.y) }) &&
			!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x), int(new_pos_y + collider.y + collider.h) }) &&
			!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x + collider.w), int(new_pos_y + collider.y) }) &&
			!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x + collider.w), int(new_pos_y + collider.y + collider.h) })) {
		ret.y = speed.y;
	}

	return ret;
}

void sl2dge::Rigidbody::start(Game *game) {
}

void sl2dge::Rigidbody::handle_events(Game *game, SDL_Event const &e) {
}

void sl2dge::Rigidbody::draw(Game *game) {
}

void sl2dge::Rigidbody::load(const pugi::xml_node &node) {
}

void sl2dge::Rigidbody::save(pugi::xml_node &node) {
}
