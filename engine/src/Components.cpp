#include "Components.h"

#include <memory>

namespace sl2dge {
	Vector2 IMoveable::pixel_position() const {
		return position_;
	}

	Point IMoveable::rounded_tiled_position(const int tile_size_) const {
		return Point(int(round(position_.x / float(tile_size_))), int(round(position_.y / float(tile_size_))));
	}

	Point IMoveable::tiled_position(const int tile_size_) const {
		return Point(position_) / tile_size_;
	}

	Direction IMoveable::facing_direction() const {
		return facing_direction_;
	}

	void IMoveable::move_and_stop(bool collision_at_next_tile) {
		if (!collision_at_next_tile)
			position_ += movement_;
		movement_ = Vector2(0, 0);
	}

	void IMoveable::move_and_slide(const TileMap& map) {

		// Move in X
		auto new_pos_x = position_.x + movement_.x;		
		if (new_pos_x > 0 &&
			!map.get_collision_at_tile(Point(int(new_pos_x + collider_.x), int(position_.y + collider_.y)) / map.tile_size()) &&
			!map.get_collision_at_tile(Point(int(new_pos_x + collider_.x + collider_.w), int(position_.y + collider_.y)) / map.tile_size()) &&
			!map.get_collision_at_tile(Point(int(new_pos_x + collider_.x), int(position_.y + collider_.y + collider_.h)) / map.tile_size()) &&
			!map.get_collision_at_tile(Point(int(new_pos_x + collider_.x + collider_.w), int(position_.y + collider_.y + collider_.h)) / map.tile_size())) {
			position_.x = new_pos_x;
		}

		// Move in Y
		auto new_pos_y = position_.y + movement_.y;
		if ( new_pos_y > 0 &&
			!map.get_collision_at_tile(Point(int(position_.x + collider_.x), int(new_pos_y + collider_.y)) / map.tile_size()) &&
			!map.get_collision_at_tile(Point(int(position_.x + collider_.x), int(new_pos_y + collider_.y + collider_.h)) / map.tile_size()) &&
			!map.get_collision_at_tile(Point(int(position_.x + collider_.x + collider_.w), int(new_pos_y + collider_.y)) / map.tile_size()) &&
			!map.get_collision_at_tile(Point(int(position_.x + collider_.x + collider_.w), int(new_pos_y + collider_.y + collider_.h)) / map.tile_size())) {
			position_.y = new_pos_y;
		}

		movement_ = Vector2(0, 0);
	}

	void IMoveable::teleport_to(const Vector2& position) {
		this->position_ = position;
	}
	void IMoveable::teleport_to(const Point& position, const int tile_size_) {
		this->position_.x = float(position.x * tile_size_);
		this->position_.y = float(position.y * tile_size_);
	}
}