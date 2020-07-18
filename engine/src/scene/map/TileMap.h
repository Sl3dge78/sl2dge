#pragma once

#include <string>

#include <SDL/SDL.h>

#include "ECS/ECS.h"
#include "addons/pugixml.hpp"
#include "scene/Camera.h"
#include "scene/map/Atlas.h"

namespace sl2dge {

class Game;

class TileMap : public Component {
public:
	TileMap(SDL_Renderer &renderer, const pugi::xml_node &map_node);
	TileMap(SDL_Renderer &renderer, const std::string &path);
	~TileMap();

	void load(SDL_Renderer &renderer, const pugi::xml_node &map_node);
	void save(pugi::xml_node &node);

	void draw(Game *game, int params);

	// Returns a rect containing all tiles to be drawn by the camera provided
	//SDL_Rect get_bounds(const Camera &camera);

	SDL_Rect map_to_pixel_transform(const SDL_Rect &rect) const { return SDL_Rect{ rect.x * tile_size_, rect.y * tile_size_, rect.h * tile_size_, rect.w * tile_size_ }; }
	SDL_Rect pixel_to_map_transform(const SDL_Rect &rect) const { return SDL_Rect{ rect.x / tile_size_, rect.y / tile_size_, rect.h / tile_size_, rect.w / tile_size_ }; }
	SDL_Point map_to_pixel_transform(const SDL_Point &point) const { return SDL_Point{ point.x * tile_size_, point.y * tile_size_ }; }
	SDL_Point pixel_to_map_transform(const SDL_Point &point) const {
		return SDL_Point{ point.x / tile_size_, point.y / tile_size_ };
	}

	// Return true if there is collision at tile position point
	bool get_collision_at_tile(const SDL_Point &point) const;
	void set_collision_at_tile(const SDL_Point &point, const bool val);

	// Changes the value of a tile
	void set_tile(const int layer, const SDL_Point &pos, const int tile);
	int get_tile(const int layer, const SDL_Point &pos);

	int tile_size() const { return tile_size_; }
	Atlas *atlas() { return atlas_; }
	unsigned int width() const { return width_; }
	unsigned int height() const { return height_; }
	void set_bounds(const SDL_Rect &bounds) { bounds_ = bounds; }
	const SDL_Rect &bounds() const { return bounds_; }

private:
	SDL_Rect bounds_{ 0, 0, 1, 1 };
	Atlas *atlas_ = nullptr;

	int *back_layer_ = nullptr;
	int *middle_layer_ = nullptr;
	int *front_layer_ = nullptr;
	bool *collision_layer_ = nullptr;

	int tile_size_ = 16;
	int width_ = 0, height_ = 0;
};

class TileMapSystem : public ISystem, public DrawSystem, public WorldSetSystem {
public:
	TileMapSystem(int params, int order = 0) {
		this->draw_params_ = params;
		this->pos_z = order;
		add_component_filter<TileMap>();
	}

	void draw();

	enum DrawParams {
		Back = 1 << 0,
		Middle = 1 << 1,
		Front = 1 << 2,
		Collision = 1 << 3
	};

private:
	int draw_params_;
};

} // namespace sl2dge
