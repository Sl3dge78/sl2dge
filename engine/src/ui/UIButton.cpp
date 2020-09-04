#include "UIButton.h"

#include "scene/Transform.h"

namespace sl2dge {

Uint32 UIButton::ON_CLICK = 0;

UIButton::UIButton() {
	if (UIButton::ON_CLICK == 0)
		UIButton::ON_CLICK = SDL_RegisterEvents(1);

	if (ON_CLICK != ((Uint32)-1)) {
		SDL_memset(&event_, 0, sizeof(event_));
		event_.type = ON_CLICK;
		event_.user.data1 = this;
	}
}

void UIButton::load(const pugi::xml_node &node) {
}

void UIButton::save(pugi::xml_node &node) {
}

void UIButton::start(Game *game) {
}

void UIButton::update(Game *game) {
}

void UIButton::handle_events(Game *game, SDL_Event const &e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			SDL_Rect pos{ transform()->position().x, transform()->position().y, w, h };
			SDL_Point mouse_pos{ e.button.x, e.button.y };
			if (SDL_PointInRect(&mouse_pos, &pos)) {
				SDL_PushEvent(&event_);
			}
		}
	}
}
void UIButton::draw(Game *game) {
}
} // namespace sl2dge
