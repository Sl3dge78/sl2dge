#pragma once

#include <vector>

#include "ECS/Component.h"
#include "math/Vector.h"

namespace sl2dge {

enum class Direction { Up,
	Down,
	Left,
	Right };

class Transform : public Component {
public:
	Transform();
	Transform(const Vector2f &position);
	Transform(const float x, const float y);

	~Transform();

	Vector2i tiled_position();
	Vector2f position() const;
	void translate(const Vector2f &translation);
	void set_position(const Vector2f &pos);
	void set_position(const int x, const int y);

	/* Parent / child */
	int order_ = 0;

	Transform *parent = nullptr;
	std::vector<Transform *> get_children() const;
	void add_children(Transform *child);
	void remove_children(Transform *child);
	void remove_all_children();

private:
	Vector2f local_position_;
	std::vector<Transform *> childs_;
};

} // namespace sl2dge
