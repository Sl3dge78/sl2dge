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
};

class SpriteSystem : public ISystem, public DrawSystem {
public:
	SpriteSystem();
	virtual void draw(Game *game) override;
};

} // namespace sl2dge
