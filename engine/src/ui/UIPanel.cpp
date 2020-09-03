#include "UIPanel.h"

sl2dge::UIPanel::UIPanel(const int w, const int h, const SDL_Color &color) {
	color_ = color;
	w_ = w;
	h_ = h;
}

void sl2dge::UIPanel::load(const pugi::xml_node &node) {
}

void sl2dge::UIPanel::save(pugi::xml_node &node) {
}

void sl2dge::UIPanel::start(Game *game) {
}

void sl2dge::UIPanel::update(Game *game) {
}

void sl2dge::UIPanel::draw(Game *game) {
	SDL_Color previous;
	SDL_GetRenderDrawColor(game->renderer(), &previous.r, &previous.g, &previous.b, &previous.a);

	SDL_SetRenderDrawColor(game->renderer(), color_.r, color_.g, color_.b, color_.a);
	SDL_Rect rect = { transform()->position().x, transform()->position().y, w_, h_ };
	SDL_RenderFillRect(game->renderer(), &rect);
	SDL_SetRenderDrawColor(game->renderer(), previous.r, previous.g, previous.b, previous.a);
}

void sl2dge::UIPanel::handle_events(Game *game, SDL_Event const &e) {
}
