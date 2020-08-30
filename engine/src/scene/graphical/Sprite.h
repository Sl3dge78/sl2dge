#pragma once

#include <string>

#include <SDL/SDL.h>

#include "ECS/ECS.h"

namespace sl2dge {

class Sprite : public Component {
public:
	Sprite(SDL_Renderer &renderer, const std::string &path, int width, int height);
	~Sprite();

	SDL_Rect src;
	SDL_Texture *texture;
	unsigned int columns;
	unsigned int rows;

	void draw(Game *game) override;

	// Inherited via Component
	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;
};

} // namespace sl2dge
