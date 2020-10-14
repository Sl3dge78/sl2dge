#pragma once

#include <string>

#include <SDL/SDL.h>

#include "ECS/ECS.h"
#include "addons/pugixml.hpp"

namespace sl2dge {
class Game;

class Map2D : public Component {
public:
	Map2D() = default;
	virtual ~Map2D();

	void load(const pugi::xml_node &node) override;
	void save(pugi::xml_node &node) override;
	void draw(Game *game) override;

	SDL_Rect map_to_pixel_transform(const SDL_Rect &rect) const { return SDL_Rect{ rect.x * tile_size_, rect.y * tile_size_, rect.h * tile_size_, rect.w * tile_size_ }; }
	SDL_Rect pixel_to_map_transform(const SDL_Rect &rect) const { return SDL_Rect{ rect.x / tile_size_, rect.y / tile_size_, rect.h / tile_size_, rect.w / tile_size_ }; }
	SDL_Point map_to_pixel_transform(const SDL_Point &point) const { return SDL_Point{ point.x * tile_size_, point.y * tile_size_ }; }
	SDL_Point pixel_to_map_transform(const SDL_Point &point) const { return SDL_Point{ point.x / tile_size_, point.y / tile_size_ }; }

	void set_tile(const SDL_Point &pos, const int tile);
	int get_tile(const SDL_Point &pos) const;

	int tile_size() const { return tile_size_; }
	int width() const { return width_; }
	int height() const { return height_; }
	void set_bounds(const SDL_Rect &bounds) { bounds_ = bounds; }
	const SDL_Rect &bounds() const { return bounds_; }

protected:
	virtual void draw_tile(Game *game, int x, int y, int tile) = 0;

private:
	SDL_Rect bounds_{ 0, 0, 1, 1 };

	int *data_ = nullptr;

	int tile_size_ = 16;
	int width_ = 100, height_ = 100;
};

} // namespace sl2dge
