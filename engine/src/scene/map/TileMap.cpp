#include "TileMap.h"

#include <fstream>
#include <sstream>
#include <string>

#include <SDL/SDL.h>

#include "Game.h"
#include "scene/Camera.h"
#include "scene/map/Atlas.h"

namespace sl2dge {

void TileMap::draw_tile(Game *game, int x, int y, int tile) {
	auto camera = Camera::main_camera;
	if (camera == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "main camera is null");
		return;
	}

	if (tile >= 0) {
		SDL_Rect dst = camera->world_to_screen_transform({ x * tile_size(), y * tile_size(), tile_size(), tile_size() });
		auto tex = atlas()->texture();
		auto tilerec = atlas()->get_tile(tile);
		SDL_RenderCopy(game->renderer(), atlas()->texture(), atlas()->get_tile(tile), &dst);
	}
}

void TileMap::update(Game *game) {
}

void TileMap::handle_events(Game *game, SDL_Event const &e) {
}

TileMap::TileMap() {
}

TileMap::~TileMap() {
	delete atlas_;
}

void TileMap::load(const pugi::xml_node &node) {
	Map2D::load(node);
	atlas_path_ = node.attribute("atlas").as_string();
}

void TileMap::save(pugi::xml_node &node) {
	Map2D::save(node);
	node.append_attribute("atlas").set_value(atlas_path_.c_str());
}

void TileMap::start(Game *game) {
	atlas_ = new Atlas(*game->renderer(), atlas_path_, tile_size());
}

// Needs to be in map coordinates (1,1 = 16,16 position)
bool CollisionMap::get_collision_at_tile(const SDL_Point &point) const {
	if (point.x >= 0 && point.x < width() && point.y >= 0 && point.y < height()) {
		return get_tile(point);
	}
	// Out of bounds
	return true;
}

bool CollisionMap::get_collision_at_pixel(const SDL_Point &point) const {
	return get_collision_at_tile(SDL_Point{ point.x / tile_size(), point.y / tile_size() });
}

void CollisionMap::set_collision_at_tile(const SDL_Point &point, const bool val) {
	set_tile(point, val);
}

void CollisionMap::draw_tile(Game *game, int x, int y, int tile) {
	if (game->current_platform() == Game::Platform::Editor) {
		SDL_SetRenderDrawColor(game->renderer(), 255, 150, 150, 255);
		SDL_RenderDrawRect(game->renderer(), &Camera::main_camera->world_to_screen_transform(SDL_Rect{ x * tile_size(), y * tile_size(), tile_size(), tile_size() }));
	}
}

void CollisionMap::start(Game *game) {
}

void CollisionMap::update(Game *game) {
}

void CollisionMap::handle_events(Game *game, SDL_Event const &e) {
}

} // namespace sl2dge
