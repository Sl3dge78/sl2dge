#include "UINumberField.h"

void sl2dge::UINumberField::draw(Game *game) {
	if (!is_editing_text) {
		FC_Draw(font_, game->renderer(), transform()->position().x, transform()->position().y, get_field_string().c_str());
		SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 255);
	} else {
		FC_Draw(font_, game->renderer(), transform()->position().x, transform()->position().y, current_text_.c_str());
		SDL_SetRenderDrawColor(game->renderer(), 255, 0, 0, 255);
	}
	SDL_RenderDrawRect(game->renderer(), &get_text_box());
}

void sl2dge::UINumberField::handle_events(Game *game, SDL_Event const &e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			auto mouse_pos = SDL_Point{ e.button.x, e.button.y };
			if (SDL_PointInRect(&mouse_pos, &get_text_box()) && !is_editing_text) { // Clicking in the text box
				is_editing_text = true;
				current_text_ = get_field_string();
				SDL_StartTextInput();
			} else if (is_editing_text) {
				is_editing_text = false;
				on_text_edit_end(current_text_);
				SDL_StopTextInput();
			}
		}
	}

	if (is_editing_text) {
		if (e.type == SDL_TEXTINPUT) {
			current_text_ += e.text.text;
		} else if (e.type == SDL_KEYDOWN) {
			//Handle backspace
			if (e.key.keysym.sym == SDLK_BACKSPACE && current_text_.length() > 0) {
				//lop off character
				current_text_.pop_back();
			}
			if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_ESCAPE) {
				is_editing_text = false;
				on_text_edit_end(current_text_);
				SDL_StopTextInput();
			}
		}
	}
}

void sl2dge::UIFloatField::on_text_edit_end(const std::string entered_text) {
	float value = *field_;
	try {
		value = std::stof(entered_text);
	} catch (std::invalid_argument &e) {
		SDL_LogWarn(0, "Unable to parse value in float field");
		return;
	}
	*field_ = value;
}

std::string sl2dge::UIFloatField::get_field_string() {
	return std::to_string(*field_);
}

void sl2dge::UIIntField::on_text_edit_end(const std::string entered_text) {
	int value = *field_;
	try {
		value = std::stoi(entered_text);
	} catch (std::invalid_argument &e) {
		SDL_LogWarn(0, "Unable to parse value in int field");
		return;
	}
	*field_ = value;
}

std::string sl2dge::UIIntField::get_field_string() {
	return std::to_string(*field_);
}
