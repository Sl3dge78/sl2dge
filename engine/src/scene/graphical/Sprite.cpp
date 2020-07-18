#include "Sprite.h"

#include <stdexcept>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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

SpriteSystem::SpriteSystem() {
	this->add_component_filter<Sprite>();
	this->add_component_filter<Transform>();
	this->set_filter_type(FilterType::FILTER_AND);

	this->pos_z = 1;
}

void SpriteSystem::draw() {
	for (auto e : entities_) {
		auto tsrfm = e->get_component<Transform>();
		auto sprite = e->get_component<Sprite>();

		SDL_Rect screen_dest = camera_->world_to_screen_transform({ int(tsrfm->position.x), int(tsrfm->position.y), sprite->src.w, sprite->src.h });
		SDL_RenderCopy(renderer_, sprite->texture, &(sprite->src), &screen_dest);
	}
}

} // namespace sl2dge
