#include "EventNodeBox.h"

#include <string>

#include "SDL_FontCache.h"

EventNodeBox::EventNodeBox(int out_amt) {

	uuid_ = newGuid();
	out_amt_ = out_amt;
	next = std::vector<Guid>(out_amt, Guid());

}

SDL_Rect EventNodeBox::in_plug() {
	if (has_in_)
		return SDL_Rect{ rect_.x - 10, rect_.y + rect_.h / 2, 10, 10 };
	else
		throw std::runtime_error("No in");
}

SDL_Rect EventNodeBox::out_plug(int i) {
	if (out_amt_ == 0) {
		throw std::runtime_error("No out");
	} else if (out_amt_ == 1) {
		return SDL_Rect{ rect_.x + rect_.w, rect_.y + rect_.h / 2, 10, 10 };
	} else {
		return SDL_Rect{ rect_.x + rect_.w, rect_.y + (rect_.h / (out_amt_ + 1) * (i + 1)), 10, 10 };
	}
}

void EventNodeBox::set_out_amt(int i) {

	if (next.size() == i)
		return;

	auto tmp = next;

	next.clear();
	for (int i = 0; i < out_amt_; i++) {
		Guid val;
		if (i < tmp.size())
			val = tmp[i];
		this->next.push_back(val);
	}
}

void EventNodeBox::draw(Game* game) {

	auto pos = game->main_camera()->world_to_screen_transform(rect_);

	// BOX
	SDL_SetRenderDrawColor(game->renderer(), 220, 220, 220, 255);
	SDL_RenderFillRect(game->renderer(), &pos);

	// Resize Corner
	SDL_SetRenderDrawColor(game->renderer(), 50, 50, 50, 255);
	SDL_RenderFillRect(game->renderer(), &game->main_camera()->world_to_screen_transform(get_corner()));

	// TITLE
	FC_Draw(game->font(), game->renderer(), float(pos.x), float(pos.y), title.c_str());

	// In
	SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
	if (has_in_) {

		if (!has_prev)
			SDL_RenderDrawRect(game->renderer(), &game->main_camera()->world_to_screen_transform(in_plug()));
		else
			SDL_RenderFillRect(game->renderer(), &game->main_camera()->world_to_screen_transform(in_plug()));
	}

	// Out
	for (int i = 0; i < next.size(); i++) {
		if (next[i].isNil())
			SDL_RenderDrawRect(game->renderer(), &game->main_camera()->world_to_screen_transform(out_plug(i)));
		else
			SDL_RenderFillRect(game->renderer(), &game->main_camera()->world_to_screen_transform(out_plug(i)));
	}
}

void EventNodeBox::handle_events(Game* game, const SDL_Event& e) {
	/*
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
		if (SDL_PointInRect(&mouse_pos, &rect_)) {
			if (e.button.button == SDL_BUTTON_LEFT) {

				if (SDL_PointInRect(&mouse_pos, &get_corner())) {
					is_resizing_ = true;
				} else {
					is_moving_ = true;
				}
			} else if (e.button.button == SDL_BUTTON_RIGHT) {
				SDL_Log("Me : %s", uuid_.str().c_str());
				for (int i = 0; i < next.size(); i++) {
					SDL_Log("Next %d : %s", i, next[i].str().c_str());
				}
			}
		}
	}
	

	if (e.type == SDL_MOUSEBUTTONUP) {
		is_resizing_ = false;
		is_moving_ = false;
	}

	if (e.type == SDL_MOUSEMOTION) {
		if (is_resizing_) {
			resize(e.motion.xrel, e.motion.yrel);
			on_box_moved();
		} else if (is_moving_) {
			rect_.x += e.motion.xrel;
			rect_.y += e.motion.yrel;
			on_box_moved();
		}
	}*/
}


void EventNodeBox::resize(int x, int y) {

	rect_.w += x;
	rect_.h += y;

	if (rect_.w < 100)
		rect_.w = 100;

	if (rect_.h < 100)
		rect_.h = 100;

	on_box_moved();
}

void EventNodeBox::translate(int x, int y) {
	rect_.x += x;
	rect_.y += y;
	on_box_moved();
}

int EventNodeBox::is_point_in_plug(SDL_Point* point) {

	for (int i = 0; i < next.size(); i++) {
		if (SDL_PointInRect(point, &out_plug(i)))
			return i;
	}
	return -1;
}


// DIALOG NODE //

void DialogNodeBox::draw(Game* game) {

	EventNodeBox::draw(game);

	// Draw text box
	if(!is_editing_text)
		SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 255);
	else
		SDL_SetRenderDrawColor(game->renderer(), 255, 0, 0, 255);
	auto pos = game->main_camera()->world_to_screen_transform(get_text_box());
	SDL_RenderDrawRect(game->renderer(), &pos);

	FC_Draw(game->font(), game->renderer(), float(pos.x), float(pos.y), text.c_str());
}

void DialogNodeBox::handle_events(Game* game, const SDL_Event& e) {

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
			if (SDL_PointInRect(&mouse_pos, &get_text_box()) && !is_editing_text) { // Clicking in the text box
				is_editing_text = true;
				SDL_StartTextInput();
			} else if (is_editing_text) {
				is_editing_text = false;
				SDL_StopTextInput();
			}
		}
	}

	if (is_editing_text) {
		if (e.type == SDL_TEXTINPUT) {
			text += e.text.text;
		} else if (e.type == SDL_KEYDOWN) {
			//Handle backspace
			if (e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
				//lop off character
				text.pop_back();
			}
			if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_ESCAPE) {
				is_editing_text = false;
				SDL_StopTextInput();
			}
		}
	}

	EventNodeBox::handle_events(game, e);
}

// TRIGGER //

void TriggerBox::draw(Game* game) {

	EventNodeBox::draw(game);

	interactable_.draw(game);
	is_in_place_.draw(game);
	activate_once_.draw(game);
	
}

void TriggerBox::handle_events(Game* game, const SDL_Event& e) {
	interactable_.handle_events(game, e);
	is_in_place_.handle_events(game, e);
	activate_once_.handle_events(game, e);

	EventNodeBox::handle_events(game, e);

}

void TriggerBox::on_box_moved() {

	SDL_Point pos = { rect_.x + 8, rect_.y + (rect_.h / 4) };
	interactable_.set_position(pos);
	pos.y += (rect_.h / 4);
	is_in_place_.set_position(pos);
	pos.y += (rect_.h / 4);
	activate_once_.set_position(pos);

}
