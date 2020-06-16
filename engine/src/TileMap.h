#pragma once

#include <string>

#include "SDL/SDL.h"

#include "Atlas.h"
#include "Camera.h"
#include "Point.h"
#include "xml/pugixml.hpp"

namespace sl2dge {

	class Game;

	class TileMap {
	public:
		TileMap(SDL_Renderer& renderer, const pugi::xml_node& map_node);
		~TileMap();

		enum DrawParams {
			Back = 1 << 0,
			Middle = 1 << 1,
			Front = 1 << 2,
			Collision = 1 << 3
		};
		void draw(Game* game, int params);

		// Returns a rect containing all tiles to be drawn by the camera provided
		Rect get_bounds(const Camera& camera);

		Rect map_to_pixel_transform(const Rect& rect) const { return Rect(rect * tile_size_); }
		Rect pixel_to_map_transform(const Rect& rect) const { return Rect(rect / tile_size_); }
		Point map_to_pixel_transform(const Point& point) const { return Point(point * tile_size_); }
		Point pixel_to_map_transform(const Point& point) const { return Point(point / tile_size_); }

		// Return true if there is collision at tile position point
		bool get_collision_at_tile(const Point& point) const;
		void set_collision_at_tile(const Point& point, const bool val);

		// Changes the value of a tile
		void set_tile(const int layer, const Point& pos, const int tile);
		int get_tile(const int layer, const Point& pos);
		void save(pugi::xml_node& node);

		int tile_size() const { return tile_size_; }
		Atlas* atlas() { return atlas_; }
		unsigned int width() const { return width_; }
		unsigned int height() const { return height_; }
		
	private:
		Atlas* atlas_ = nullptr;

		int* back_layer_ = nullptr;
		int* middle_layer_ = nullptr;
		int* front_layer_ = nullptr;
		bool* collision_layer_ = nullptr;

		int tile_size_ = 16;
		int width_ = 0, height_ = 0;

	};
}

