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
	COMPONENT_DFLT_BEHAVIOUR(Transform)

public:
	Transform();
	Transform(const Vector2f &position);
	Transform(const float x, const float y);

	~Transform();

	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;

	/// World position
	Vector2f position() const;
	// Position in map units
	Vector2i tiled_position();

	int z = 0;

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

	// Inherited via Component
	virtual void start(Game *game) override;
	virtual void update(Game *game) override;
	virtual void handle_events(Game *game, SDL_Event const &e) override;
	virtual void draw(Game *game) override;
};

} // namespace sl2dge
