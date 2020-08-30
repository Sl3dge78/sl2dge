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
	Transform(const pugi::xml_node &node) { load(node); };
	Transform(const Vector2f &position);
	Transform(const float x, const float y);

	~Transform();

	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;

	/// World position
	Vector2f position() const;
	// Position in map units
	Vector2i tiled_position();

	void translate(const Vector2f &translation);
	void set_position(const Vector2f &pos);
	void set_position(const float x, const float y);

	/* Parent / child */
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
