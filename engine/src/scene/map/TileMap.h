#pragma once

#include <string>

#include <SDL/SDL.h>

#include "ECS/ECS.h"
#include "addons/pugixml.hpp"
#include "scene/Camera.h"
#include "scene/map/Atlas.h"
#include "scene/map/Map2D.h"

namespace sl2dge {

class Game;

class TileMap : public Map2D {
public:
	TileMap(const pugi::xml_node &node);
	~TileMap();

	void start(Game *game);
	Atlas *atlas() { return atlas_; }

private:
	std::string atlas_path_;
	Atlas *atlas_ = nullptr;

	void draw_tile(Game *game, int x, int y, int tile) override;
};

class CollisionMap : public Map2D {
public:
	CollisionMap(const pugi::xml_node &node) {
		load(node);
	};
	// Return true if there is collision at tile position point
	bool get_collision_at_tile(const SDL_Point &point) const;
	bool get_collision_at_pixel(const SDL_Point &point) const;
	void set_collision_at_tile(const SDL_Point &point, const bool val);
	void draw_tile(Game *game, int x, int y, int tile) override;
};

} // namespace sl2dge
