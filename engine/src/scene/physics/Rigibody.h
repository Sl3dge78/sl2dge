#pragma once

#include "ECS/ECS.h"
#include "addons/pugixml.hpp"
#include "math/Vector.h"
#include "scene/map/TileMap.h"

namespace sl2dge {

class Rigidbody : public Component {
public:
	Rigidbody(){};

	Vector2f speed = Vector2f(0, 0);
	SDL_Rect collider = SDL_Rect{ 0, 0, 16, 16 };

	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;

	void update(Game *game) override;

	void set_map(CollisionMap *tilemap) { map_ = tilemap; }

private:
	CollisionMap *map_ = nullptr;
	Vector2f move_and_slide();

	// Inherited via Component
	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, SDL_Event const &e) override;
	virtual void draw(Game *game) override;
};
} // namespace sl2dge
