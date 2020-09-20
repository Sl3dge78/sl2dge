#include "Sprite.h"

#include <stdexcept>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Game.h"
#include "scene/Camera.h"
#include "scene/Transform.h"

namespace sl2dge {

Sprite::Sprite(const std::string &path, const int width, const int height) {
	path_ = path;
	width_ = width;
	height_ = height;
}

Sprite::Sprite() {
}

Sprite::~Sprite() {
	SDL_DestroyTexture(texture_);
}

void Sprite::start(Game *game) {
	if (path_ == "")
		return;

	SDL_Surface *surf = IMG_Load(path_.c_str());
	if (surf == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error reading file %s : %s", path_, IMG_GetError());
		throw std::runtime_error(IMG_GetError());
	}

	texture_ = SDL_CreateTextureFromSurface(game->renderer(), surf);
	src = { 0, 0, width_, height_ };
	columns_ = surf->w / width_;
	rows_ = surf->h / height_;
	SDL_FreeSurface(surf);
}

void Sprite::draw(Game *game) {
	auto tsrfm = entity()->transform();

	SDL_Rect screen_dest = Camera::main_camera->world_to_screen_transform({ int(tsrfm->position().x), int(tsrfm->position().y), src.w, src.h });
	SDL_RenderCopy(game->renderer(), texture_, &(src), &screen_dest);
}

void Sprite::load(const pugi::xml_node &node) {
	width_ = node.attribute("width").as_int();
	height_ = node.attribute("height").as_int();
	path_ = node.attribute("path").as_string();
}

void Sprite::save(pugi::xml_node &node) {
	node.append_attribute("width").set_value(width_);
	node.append_attribute("height").set_value(height_);
	node.append_attribute("path").set_value(path_.c_str());
}

} // namespace sl2dge
