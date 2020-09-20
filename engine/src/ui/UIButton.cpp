#include "UIButton.h"

#include "scene/Transform.h"

namespace sl2dge {

UIButton::UIButton(std::function<void()> on_click) {
	on_click_ = on_click;
}

UIButton::UIButton(std::function<void()> on_click, int w, int h) {
	on_click_ = on_click;
	this->w = w;
	this->h = h;
}

void UIButton::handle_events(Game *game, SDL_Event const &e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			SDL_Rect pos{ int(transform()->position().x), int(transform()->position().y), w, h };
			SDL_Point mouse_pos{ e.button.x, e.button.y };
			if (SDL_PointInRect(&mouse_pos, &pos)) {
				on_click_();
			}
		}
	}
}
} // namespace sl2dge
