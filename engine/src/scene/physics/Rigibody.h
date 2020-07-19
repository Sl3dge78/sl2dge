#pragma once

#include "ECS/ECS.h"
#include "math/Vector.h"
#include "scene/map/TileMap.h"

namespace sl2dge {

class Rigidbody : public Component {
public:
	Vector2f speed = Vector2f(0, 0);
	SDL_Rect collider = SDL_Rect{ 0, 0, 16, 16 };
};

class PhysicsSystem : public ISystem, public UpdateSystem {
public:
	PhysicsSystem(TileMap *map) {
		this->add_component_filter<Rigidbody>();
		map_ = map;
	}

	void update(Game *game) override {
		for (auto &e : entities_) {
			auto rb = e->get_component<Rigidbody>();
			if (!rb->speed.is_zero()) {
				auto translation = move_and_slide(rb);
				e->transform()->translate(translation);
				rb->speed = Vector2f(0, 0);
			}
		}
	}

private:
	TileMap *map_;

	Vector2f move_and_slide(Rigidbody *rb) const {
		// Move in X
		const auto position = rb->entity()->transform()->position();
		const auto collider = rb->collider;

		Vector2f ret = Vector2f(0, 0);

		auto new_pos_x = position.x + rb->speed.x;
		if (new_pos_x > 0 &&
				!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x), int(position.y + collider.y) }) &&
				!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x + collider.w), int(position.y + collider.y) }) &&
				!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x), int(position.y + collider.y + collider.h) }) &&
				!map_->get_collision_at_pixel(SDL_Point{ int(new_pos_x + collider.x + collider.w), int(position.y + collider.y + collider.h) })) {
			ret.x = rb->speed.x;
		}

		// Move in Y
		auto new_pos_y = position.y + rb->speed.y;
		if (new_pos_y > 0 &&
				!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x), int(new_pos_y + collider.y) }) &&
				!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x), int(new_pos_y + collider.y + collider.h) }) &&
				!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x + collider.w), int(new_pos_y + collider.y) }) &&
				!map_->get_collision_at_pixel(SDL_Point{ int(position.x + collider.x + collider.w), int(new_pos_y + collider.y + collider.h) })) {
			ret.y = rb->speed.y;
		}

		return ret;
	}
};

} // namespace sl2dge
