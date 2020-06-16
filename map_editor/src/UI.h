#include "sl2dge.h"

using namespace sl2dge;

class ToggleBox {

public:
	
	ToggleBox(const std::string& name, bool controlled_val) {
		value_ = controlled_val;
		name_ = name;
	}

	void draw(Game* game) {
		SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 255);
		if (!value_)
			SDL_RenderDrawRect(game->renderer(), &box_);
		else
			SDL_RenderFillRect(game->renderer(), &box_);
		
		FC_Draw(game->font(), game->renderer(), float(text_.x), float(text_.y), name_.c_str());
	}

	void handle_events(Game* game, const SDL_Event& e) {
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
				if (SDL_PointInRect(&mouse_pos, &box_)) { // Clicking in the toggle box
					value_ = !value_;
				}
			}
		}
	}

	void set_position(const SDL_Point& position) {
		box_ = SDL_Rect{ position.x, position.y + 3, 10, 10 };
		text_ = SDL_Point{ position.x + 12, position.y };
	}

	bool const value() { return value_; }
	void value(bool b) { value_ = b; }

private:

	std::string name_;
	bool value_;

	SDL_Rect box_ = { 0, 0, 10, 10 };
	SDL_Point text_ = { 0, 0 };

};