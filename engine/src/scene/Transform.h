#pragma once

#include "ECS/Component.h"
#include "math/Vector.h"

namespace sl2dge {

class Transform : public Component {
public:
	Vector2f position;

	Transform(const float x, const float y) {
		position = Vector2f(x, y);
	}
};

} // namespace sl2dge
