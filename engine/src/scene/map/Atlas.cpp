#include "Atlas.h"

#include <stdexcept>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace sl2dge {
Atlas::Atlas(SDL_Renderer &renderer, const std::string &atlas_path, const int tile_size_,
		const int nb_tiles_atlas_width, const int nb_tiles_atlas_height) {
	load_texture(renderer, atlas_path);
	load_tiles(tile_size_, nb_tiles_atlas_width, nb_tiles_atlas_height);
	this->tile_size_ = tile_size_;
	this->nb_tiles_atlas_width = nb_tiles_atlas_width;
	this->nb_tiles_atlas_height = nb_tiles_atlas_height;
	this->path = atlas_path;
}

Atlas::Atlas(SDL_Renderer &renderer, const std::string &atlas_path, const int tile_size_) {
	this->tile_size_ = tile_size_;
	load_texture(renderer, atlas_path);
	load_tiles(tile_size_, nb_tiles_atlas_width, nb_tiles_atlas_height);
	this->path = atlas_path;
}

Atlas::~Atlas() {
	delete[] tiles_;
	SDL_DestroyTexture(texture_);
}

void Atlas::load_texture(SDL_Renderer &renderer, const std::string &atlas_path) {
	SDL_Surface *surf = IMG_Load(atlas_path.c_str());
	if (surf == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error reading file %s : %s", atlas_path, IMG_GetError());
		throw std::runtime_error(IMG_GetError());
	}
	this->nb_tiles_atlas_width = surf->w / tile_size_;
	this->nb_tiles_atlas_height = surf->h / tile_size_;
	this->texture_ = SDL_CreateTextureFromSurface(&renderer, surf);
	SDL_FreeSurface(surf);
}
void Atlas::load_tiles(const int tile_size_, const int nb_tiles_atlas_width, const int nb_tiles_atlas_height) {
	this->tiles_ = new SDL_Rect[nb_tiles_atlas_width * nb_tiles_atlas_height];
	for (int i = 0; i < nb_tiles_atlas_width * nb_tiles_atlas_height; ++i) {
		this->tiles_[i] = SDL_Rect{ (i % nb_tiles_atlas_width) * tile_size_, (i / nb_tiles_atlas_width) * tile_size_, tile_size_, tile_size_ };
	}
}
} // namespace sl2dge
