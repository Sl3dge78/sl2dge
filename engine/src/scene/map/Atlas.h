#pragma once

#include <string>

#include "SDL/SDL.h"

namespace sl2dge {
class Atlas {
public:
	Atlas(SDL_Renderer &renderer, const std::string &atlas_path, const int tile_size_, const int nb_tiles_atlas_width, const int nb_tiles_atlas_height);
	Atlas(SDL_Renderer &renderer, const std::string &atlas_path, const int tile_size_);
	~Atlas();
	SDL_Texture *texture() { return texture_; }
	SDL_Rect *get_tile(const int pos) { return &tiles_[pos]; }
	int tile_size_, nb_tiles_atlas_width, nb_tiles_atlas_height;
	std::string path;

private:
	SDL_Texture *texture_ = nullptr;
	SDL_Rect *tiles_ = nullptr;

	void load_texture(SDL_Renderer &renderer, const std::string &atlas_path);
	void load_tiles(const int tile_size_, const int nb_tiles_atlas_width, const int nb_tiles_atlas_height);
};
} // namespace sl2dge
