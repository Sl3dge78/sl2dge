#include "UIContextMenu.h"

namespace sl2dge {

UIContextMenu::UIContextMenu(int w, int h, const SDL_Color &color) :
		UIPanel(w, h, color) {
}

void UIContextMenu::load(const pugi::xml_node &node) {
}
void UIContextMenu::save(pugi::xml_node &node) {
}
void UIContextMenu::handle_events(Game *game, SDL_Event const &e) {
	if (e.type == SDL_MOUSEBUTTONUP) {
		SDL_Rect pos{ int(transform()->position().x), int(transform()->position().y), w_, h_ };
		SDL_Point mouse_pos{ e.button.x, e.button.y };
		if (!SDL_PointInRect(&mouse_pos, &pos)) {
			entity()->world()->delete_entity(entity());
		}
	}
}
} // namespace sl2dge
