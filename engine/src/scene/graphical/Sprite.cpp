#include "Sprite.h"

#include <stdexcept>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Game.h"
#include "scene/Camera.h"
#include "scene/Transform.h"

namespace sl2dge {

Sprite::Sprite(SDL_Renderer &renderer, const std::string &path, int width, int height) {
	SDL_Surface *surf = IMG_Load(path.c_str());
	if (surf == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error reading file %s : %s", path, IMG_GetError());
		throw std::runtime_error(IMG_GetError());
	}

	texture = SDL_CreateTextureFromSurface(&renderer, surf);
	src = { 0, 0, width, height };
	columns = surf->w / width;
	rows = surf->h / height;
	SDL_FreeSurface(surf);
}

Sprite::~Sprite() {
	SDL_DestroyTexture(texture);
}

void Sprite::draw(Game *game) {
	auto tsrfm = entity()->transform();

	SDL_Rect screen_dest = Camera::main_camera->world_to_screen_transform({ int(tsrfm->position().x), int(tsrfm->position().y), src.w, src.h });
	SDL_RenderCopy(game->renderer(), texture, &(src), &screen_dest);
}

void Sprite::load(const pugi::xml_node &node) {
}

void Sprite::save(pugi::xml_node &node) {
}

} // namespace sl2dge
