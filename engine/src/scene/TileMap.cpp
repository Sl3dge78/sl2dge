#include "TileMap.h"

#include <fstream>
#include <sstream>
#include <string>

#include <SDL/SDL.h>

#include "scene/Atlas.h"
#include "scene/Camera.h"

namespace sl2dge {

TileMap::TileMap(SDL_Renderer &renderer, const pugi::xml_node &map_node) {
	load(renderer, map_node);
}

TileMap::TileMap(SDL_Renderer &renderer, const std::string &path) {
	pugi::xml_document doc;
	open_xml_doc(&doc, path);

	auto map_node = doc.child("Map");
	load(renderer, map_node);
}

void TileMap::load(SDL_Renderer &renderer, const pugi::xml_node &map_node) {
	if (!map_node) {
		// Throw error
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No map data to read");
		throw std::exception("No map data to read!");
	}
	std::string atlas_path = map_node.attribute("tile_map").as_string();
	tile_size_ = map_node.attribute("tile_size").as_int();
	atlas_ = new Atlas(renderer, atlas_path, tile_size_);
	width_ = map_node.attribute("width").as_int();
	height_ = map_node.attribute("height").as_int();

	int tmp;

	std::stringstream text = std::stringstream(map_node.child("Layer_1").text().as_string());
	this->back_layer_ = new int[int(width_ * height_)]{};
	for (int i = 0; i < width_ * height_; ++i) {
		text >> tmp;
		this->back_layer_[i] = tmp;
	}

	text = std::stringstream(map_node.child("Layer_2").text().as_string());
	this->middle_layer_ = new int[int(width_ * height_)]{};
	for (int i = 0; i < width_ * height_; ++i) {
		text >> tmp;
		this->middle_layer_[i] = tmp;
	}

	text = std::stringstream(map_node.child("Layer_3").text().as_string());
	this->front_layer_ = new int[int(width_ * height_)]{};
	for (int i = 0; i < width_ * height_; ++i) {
		text >> tmp;
		this->front_layer_[i] = tmp;
	}

	text = std::stringstream(map_node.child("Collision").text().as_string());
	this->collision_layer_ = new bool[int(width_ * height_)]{};
	for (int i = 0; i < width_ * height_; ++i) {
		text >> this->collision_layer_[i];
	}
}

void TileMap::save(pugi::xml_node &node) {
	// Cleanup
	node.remove_attributes();
	node.remove_children();

	node.append_attribute("tile_map").set_value(atlas_->path.c_str());
	node.append_attribute("tile_size").set_value(tile_size_);

	node.append_attribute("width").set_value(width_);
	node.append_attribute("height").set_value(height_);

	std::stringstream text = std::stringstream();
	for (int i = 0; i < width_ * height_; ++i) {
		text << this->back_layer_[i] << " ";
	}
	auto layer_1 = node.append_child("Layer_1");
	layer_1.text() = text.str().c_str();

	text = std::stringstream();
	for (int i = 0; i < width_ * height_; ++i) {
		text << this->middle_layer_[i] << " ";
	}
	auto layer_2 = node.append_child("Layer_2");
	layer_2.text() = text.str().c_str();

	text = std::stringstream();
	for (int i = 0; i < width_ * height_; ++i) {
		text << this->front_layer_[i] << " ";
	}
	auto layer_3 = node.append_child("Layer_3");
	layer_3.text() = text.str().c_str();

	text = std::stringstream();
	for (int i = 0; i < width_ * height_; ++i) {
		text << this->collision_layer_[i] << " ";
	}
	auto collision = node.append_child("Collision");
	collision.text() = text.str().c_str();
}

TileMap::~TileMap() {
	delete[] back_layer_;
	delete[] middle_layer_;
	delete[] front_layer_;
	delete[] collision_layer_;
	delete atlas_;
}

// Needs to be in map coordinates (1,1 = 16,16 position)
bool TileMap::get_collision_at_tile(const SDL_Point &point) const {
	if (point.x >= 0 && point.x < width_ && point.y >= 0 && point.y < height_) {
		return collision_layer_[point.x + width_ * point.y];
	}
	// Out of bounds
	return true;
}

void TileMap::set_collision_at_tile(const SDL_Point &point, const bool val) {
	if (point.x >= 0 && point.x < width_ && point.y >= 0 && point.y < height_) {
		collision_layer_[point.x + width_ * point.y] = val;
	}
}

void TileMap::set_tile(const int layer, const SDL_Point &pos, const int tile) {
	if (layer == 0)
		back_layer_[pos.x + pos.y * width_] = tile;
	if (layer == 1)
		middle_layer_[pos.x + pos.y * width_] = tile;
	if (layer == 2)
		front_layer_[pos.x + pos.y * width_] = tile;
}

int TileMap::get_tile(const int layer, const SDL_Point &pos) {
	if (layer == 0)
		return back_layer_[pos.x + pos.y * width_];
	if (layer == 1)
		return middle_layer_[pos.x + pos.y * width_];
	if (layer == 2)
		return front_layer_[pos.x + pos.y * width_];
	else
		return -1;
}

void TileMapSystem::draw() {
	if (camera_ == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No camera assigned to TileMapDrawer");
		return;
	}

	auto cam = camera_->get_component<Camera>();

	for (auto e : entities_) {
		auto map = e->get_component<TileMap>();

		SDL_Point first_tile = { cam->viewport().x / map->tile_size(), cam->viewport().y / map->tile_size() };

		SDL_Point last_tile = {
			first_tile.x + (cam->viewport().w / map->tile_size()) + 1,
			first_tile.y + (cam->viewport().h / map->tile_size()) + 1
		};

		if (first_tile.x < 0)
			first_tile.x = 0;
		if (first_tile.y < 0)
			first_tile.y = 0;

		if (last_tile.x > map->width())
			last_tile.x = map->width();
		if (last_tile.y > map->height())
			last_tile.y = map->height();

		SDL_Rect bounds = { first_tile.x, first_tile.y, last_tile.x, last_tile.y };

		//Draw each tile from first tile to max rect
		for (int y = bounds.y; y < bounds.h; ++y) {
			for (int x = bounds.x; x < bounds.w; ++x) {
				if (draw_params_ & DrawParams::Back) {
					auto tile = map->get_tile(0, { x, y });
					if (tile != -1) {
						SDL_Rect dst = cam->world_to_screen_transform({ x * map->tile_size(), y * map->tile_size(), map->tile_size(), map->tile_size() });
						auto tex = map->atlas()->texture();
						auto tilerec = map->atlas()->get_tile(tile);
						SDL_RenderCopy(renderer_, map->atlas()->texture(), map->atlas()->get_tile(tile), &dst);
					}
				}
				if (draw_params_ & DrawParams::Middle) {
					auto tile = map->get_tile(1, { x, y });
					if (tile != -1) {
						SDL_Rect dst = cam->world_to_screen_transform({ x * map->tile_size(), y * map->tile_size(), map->tile_size(), map->tile_size() });
						SDL_RenderCopy(renderer_, map->atlas()->texture(), map->atlas()->get_tile(tile), &dst);
					}
				}
				if (draw_params_ & DrawParams::Front) {
					auto tile = map->get_tile(2, { x, y });
					if (tile != -1) {
						SDL_Rect dst = cam->world_to_screen_transform({ x * map->tile_size(), y * map->tile_size(), map->tile_size(), map->tile_size() });
						SDL_RenderCopy(renderer_, map->atlas()->texture(), map->atlas()->get_tile(tile), &dst);
					}
				}

				if (draw_params_ & DrawParams::Collision) {
					/* TODO
						if (collision_layer_[x + y * width_] != 0) {
							SDL_SetRenderDrawColor(game->renderer(), 255, 150, 150, 255);
							SDL_RenderDrawRect(game->renderer(), &game->main_camera()->world_to_screen_transform(SDL_Rect{ x * tile_size_, y * tile_size_, tile_size_, tile_size_ }));
						}
						*/
				}
			}
		}
	}
}
} // namespace sl2dge
