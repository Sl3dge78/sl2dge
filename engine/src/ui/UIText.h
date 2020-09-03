#pragma once

#include "sl2dge.h"

namespace sl2dge {

class UIText : public Component {
public:
	UIText() = default;
	UIText(const std::string &text, FC_Font *font = nullptr) {
		text_ = text;
		font_ = font;
	};

	// Inherited via Component
	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;
	virtual void start(Game *game) override;
	virtual void update(Game *game) override;
	virtual void handle_events(Game *game, SDL_Event const &e) override;
	virtual void draw(Game *game) override;

private:
	FC_Font *font_ = nullptr;
	std::string text_ = "";
};

} // namespace sl2dge
