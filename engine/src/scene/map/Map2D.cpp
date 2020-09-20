#include "Map2D.h"

#include <sstream>

#include "scene/Camera.h"

namespace sl2dge {

Map2D::~Map2D() {
	delete[] data_;
}
void Map2D::load(const pugi::xml_node &node) {
	if (!node) {
		// Throw error
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No map data to read");
		throw std::exception("No map data to read!");
	}

	tile_size_ = node.attribute("tile_size").as_int();
	width_ = node.attribute("width").as_int();
	height_ = node.attribute("height").as_int();

	int tmp;

	std::stringstream text = std::stringstream(node.child("Data").text().as_string());
	this->data_ = new int[width_ * height_]{};
	for (int i = 0; i < width_ * height_; ++i) {
		text >> tmp;
		this->data_[i] = tmp;
	}
}
void Map2D::save(pugi::xml_node &node) {
	node.append_attribute("tile_size").set_value(tile_size_);

	node.append_attribute("width").set_value(width_);
	node.append_attribute("height").set_value(height_);

	std::stringstream text = std::stringstream();
	for (int i = 0; i < width_ * height_; ++i) {
		text << this->data_[i] << " ";
	}
	auto data = node.append_child("Data");
	data.text() = text.str().c_str();
}
void Map2D::draw(Game *game) {
	auto camera = Camera::main_camera;
	if (camera == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "main camera is null");
		return;
	}

	SDL_Point first_tile = { camera->viewport_.x / tile_size(), camera->viewport_.y / tile_size() };

	SDL_Point last_tile = {
		first_tile.x + (camera->viewport_.w / tile_size()) + 1,
		first_tile.y + (camera->viewport_.h / tile_size()) + 1
	};

	if (first_tile.x < 0)
		first_tile.x = 0;
	if (first_tile.y < 0)
		first_tile.y = 0;

	if (last_tile.x > width())
		last_tile.x = width();
	if (last_tile.y > height())
		last_tile.y = height();

	SDL_Rect bounds = { first_tile.x, first_tile.y, last_tile.x, last_tile.y };

	//Draw each tile from first tile to max rect
	for (int y = bounds.y; y < bounds.h; ++y) {
		for (int x = bounds.x; x < bounds.w; ++x) {
			int tile = data_[x + y * width_];
			draw_tile(game, x, y, tile);
		}
	}
}
void Map2D::set_tile(const SDL_Point &pos, const int tile) {
	data_[pos.x + pos.y * width_] = tile;
}
int Map2D::get_tile(const SDL_Point &pos) const {
	return data_[pos.x + pos.y * width_];
}
} // namespace sl2dge
