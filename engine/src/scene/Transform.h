#pragma once

#include "ECS/Component.h"
#include "math/Vector.h"

namespace sl2dge {

enum class Direction { Up,
	Down,
	Left,
	Right };

class Transform : public Component {
public:
	Transform() { position = Vector2f(0, 0); }
	Transform(const Vector2f &position) { this->position = position; }
	Transform(const float x, const float y) {
		position = Vector2f(x, y);
	}

	Vector2f position;
	Vector2i tiled_position() { return Vector2i(int(position.x) / 16, int(position.y) / 16); }
};

} // namespace sl2dge
