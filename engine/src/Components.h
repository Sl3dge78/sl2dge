#pragma once

#include <memory>

#include "HelperFunctions.h"
#include "Point.h"
#include "Rect.h"
#include "TileMap.h"

namespace sl2dge {

class IMoveable {
public:
	IMoveable() {}
	Vector2 pixel_position() const;
	Point rounded_tiled_position(const int tile_size_ = 16) const;
	Point tiled_position(const int tile_size_ = 16) const;
	Direction facing_direction() const;

	void move_and_stop(bool collision_at_next_tile);
	void move_and_slide(const TileMap &map);
	void teleport_to(const Vector2 &position);
	void teleport_to(const Point &position, const int tile_size_ = 16);

protected:
	Direction facing_direction_ = Direction::Down;
	Vector2 movement_;
	Vector2 position_; // Position in pixels
	Rect collider_ = Rect(0, 0, 16, 16);
};

} // namespace sl2dge
