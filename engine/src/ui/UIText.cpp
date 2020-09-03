#include "UIText.h"

void sl2dge::UIText::load(const pugi::xml_node &node) {
}

void sl2dge::UIText::save(pugi::xml_node &node) {
}

void sl2dge::UIText::start(Game *game) {
	if (font_ == nullptr)
		font_ = game->font();
}

void sl2dge::UIText::update(Game *game) {
}

void sl2dge::UIText::handle_events(Game *game, SDL_Event const &e) {
}

void sl2dge::UIText::draw(Game *game) {
	FC_Draw(font_, game->renderer(), transform()->position().x, transform()->position().y, text_.c_str());
}
