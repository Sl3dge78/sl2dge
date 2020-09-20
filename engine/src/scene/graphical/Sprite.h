#pragma once

#include <string>

#include <SDL/SDL.h>

#include "ECS/ECS.h"

namespace sl2dge {

class Sprite : public Component {
	COMPONENT_DFLT_BEHAVIOUR(Sprite);

public:
	Sprite(const std::string &path, const int width, const int height);
	Sprite();
	~Sprite();

	void start(Game *game) override;
	void draw(Game *game) override;

	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;

	SDL_Rect src = { 0, 0, 16, 16 };
	unsigned int columns() { return columns_; }
	unsigned int rows() { return rows_; }

private:
	int width_ = 16;
	int height_ = 16;

	std::string path_ = "";
	SDL_Texture *texture_ = nullptr;
	unsigned int columns_ = 1;
	unsigned int rows_ = 1;
};

} // namespace sl2dge
