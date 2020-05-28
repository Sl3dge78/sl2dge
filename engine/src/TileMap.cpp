#include "TileMap.h"

#include <iostream>
#include <fstream>
#include <string>

#include "SDL/SDL.h"

#include "Point.h"
#include "Camera.h"
#include "Atlas.h"
#include "HelperFunctions.h"
#include "Game.h"

namespace sl2dge {
	TileMap::TileMap(SDL_Renderer& renderer, const std::string& map_path) {
		using namespace std;

		ifstream file;
		file.open(map_path);
		if (!file.is_open()) {
			cout << "Error reading file: " << map_path << endl;
			return;
		}
		string atlas_path;
		int nb_tiles_atlas_width, nb_tiles_atlas_height;

		file >> atlas_path;
		file >> this->tile_size_;
		file >> nb_tiles_atlas_width;
		file >> nb_tiles_atlas_height;

		this->atlas_ = new Atlas(renderer, atlas_path, tile_size_, nb_tiles_atlas_width, nb_tiles_atlas_height);

		file >> this->width_;
		file >> this->height_;

		int tmp;

		this->back_layer_ = new int[width_ * height_]{};
		for (int i = 0; i < width_ * height_; ++i) {
			file >> tmp;
			this->back_layer_[i] = tmp;
		}

		this->middle_layer_ = new int[width_ * height_]{};
		for (int i = 0; i < width_ * height_; ++i) {
			file >> tmp;
			this->middle_layer_[i] = tmp;
		}

		this->front_layer_ = new int[width_ * height_]{};
		for (int i = 0; i < width_ * height_; ++i) {
			file >> tmp;
			this->front_layer_[i] = tmp;
		}

		this->collision_layer_ = new bool[width_ * height_]{};
		for (int i = 0; i < width_ * height_; ++i) {
			file >> this->collision_layer_[i];
		}

		file.close();
	}

	TileMap::~TileMap() {
		delete[] back_layer_;
		delete[] middle_layer_;
		delete[] front_layer_;
		delete[] collision_layer_;
		delete atlas_;
	}
	void TileMap::draw(Game* game, int draw_params) {

		Rect bounds = get_bounds(*game->main_camera());

		//Draw each tile from first tile to max rect
		for (int y = bounds.y; y < bounds.h; ++y) {
			for (int x = bounds.x; x < bounds.w; ++x) {

				if (draw_params & DrawParams::Back) {
					if (back_layer_[x + y * width_] != -1) {
						Rect* src = atlas_->get_tile_rect(back_layer_[x + y * width_]);
						RenderCopy(*game->renderer(), *atlas_->texture(), src,
							&game->main_camera()->world_to_screen_transform(Rect(x * tile_size_, y * tile_size_, tile_size_, tile_size_)));
					}
				}
				if (draw_params & DrawParams::Middle) {
					if (middle_layer_[x + y * width_] != -1) {
						Rect* src = atlas_->get_tile_rect(middle_layer_[x + y * width_]);
						RenderCopy(*game->renderer(), *atlas_->texture(), src,
							&game->main_camera()->world_to_screen_transform(Rect(x * tile_size_, y * tile_size_, tile_size_, tile_size_)));
					}
				}
				if (draw_params & DrawParams::Front) {
					if (front_layer_[x + y * width_] != -1) {
						Rect* src = atlas_->get_tile_rect(front_layer_[x + y * width_]);
						RenderCopy(*game->renderer(), *atlas_->texture(), src,
							&game->main_camera()->world_to_screen_transform(Rect(x * tile_size_, y * tile_size_, tile_size_, tile_size_)));
					}
				}
			}
		}

	}
	Rect TileMap::get_bounds(const Camera& camera) {
		//Find first tile to draw
		Point first_tile = Point(camera.viewport().x, camera.viewport().y) / tile_size_;
		Point last_tile = first_tile + Point(camera.viewport().w, camera.viewport().h) / tile_size_ + Point(2, 2);

		if (first_tile.x < 0)
			first_tile.x = 0;
		if (first_tile.y < 0)
			first_tile.y = 0;

		if (last_tile.x > width_)
			last_tile.x = width_;
		if (last_tile.y > height_)
			last_tile.y = height_;

		return Rect(first_tile.x, first_tile.y, last_tile.x, last_tile.y);
	}

	// Needs to be in map coordinates (1,1 = 16,16 position)
	bool TileMap::get_collision_at_tile(const Point& point) const {
		if (point.x >= 0 && point.x < width_ && point.y >= 0 && point.y < height_) {
			return collision_layer_[point.x + width_ * point.y];
		}
		// Out of bounds 
		return true;
	}

	void TileMap::set_collision_at_tile(const Point& point, const bool val) {
		if (point.x > 0 && point.x < width_ && point.y > 0 && point.y < height_) {
			collision_layer_[point.x + width_ * point.y] = val;
		}
	}

	void TileMap::set_tile(const int layer, const Point& pos, const int tile) {
		if (layer == 0)
			back_layer_[pos.x + pos.y * width_] = tile;
		if (layer == 1)
			middle_layer_[pos.x + pos.y * width_] = tile;
		if (layer == 2)
			front_layer_[pos.x + pos.y * width_] = tile;
	}

	void TileMap::save(const std::string& map_path) {
		using namespace std;

		ofstream file;
		file.open(map_path);

		if (!file.is_open()) {
			cout << "Error opening file: " << map_path << endl;
			return;
		}

		file << this->atlas_->path << " ";
		file << this->tile_size_ << " ";
		file << atlas_->nb_tiles_atlas_width << " ";
		file << atlas_->nb_tiles_atlas_height << " ";
		file << this->width_ << " ";
		file << this->height_ << " ";
		file << endl;
		for (int i = 0; i < width_ * height_; ++i) {
			file << this->back_layer_[i] << " ";
		}
		file << endl;
		
		for (int i = 0; i < width_ * height_; ++i) {
			file << this->middle_layer_[i] << " ";
		}
		file << endl;
		
		for (int i = 0; i < width_ * height_; ++i) {
			file << this->front_layer_[i] << " ";
		}
		file << endl;
		
		for (int i = 0; i < width_ * height_; ++i) {
			file << this->collision_layer_[i] << " ";
		}
		file.close();
	}
}
