#include "EventNodeBox.h"

#include <string>

#include "SDL_FontCache.h"

EventNodeBox::EventNodeBox(int const out_amt, pugi::xml_node* const node) {


	if (node != nullptr) {
		out_amt_ = node->attribute("amt_next").as_int();
		//next = std::vector<Guid>(out_amt, Guid());
		for (int i = 0; i < out_amt_; ++i) {
			std::stringstream attr;
			attr << "next_" << i;
			next.push_back(Guid(node->attribute(attr.str().c_str()).as_string()));
			
		}

		rect_.x = node->attribute("x").as_int();
		rect_.y = node->attribute("y").as_int();
		rect_.w = node->attribute("w").as_int();
		rect_.h = node->attribute("h").as_int();

		resize(0, 0); // To avoid having boxes with 0 size

		std::string guid = node->attribute("guid").as_string();

		if (guid != "") {
			uuid_ = Guid(guid);
			return;
		}
	} else {
		out_amt_ = out_amt;
		next = std::vector<Guid>(out_amt, Guid());
		uuid_ = newGuid();
	}
	
}

std::unique_ptr<EventNodeBox> EventNodeBox::create_node(pugi::xml_node& node) {

	std::string type = node.attribute("type").as_string();

	if (type == "trigger") {
		return std::move(std::make_unique<TriggerBox>(node));
	}else if (type == "dialog") {
		return std::move(std::make_unique<DialogNodeBox>(node));
	} else {
		SDL_Log("Type %s unknown", type);
		throw std::exception("Type unknown");
	}
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
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
		if (e.button.button == SDL_BUTTON_RIGHT) {
			if (SDL_PointInRect(&mouse_pos, rect())) {
				SDL_Log("===\nMe : %s", guid().str().c_str());
				SDL_Log("Next : %s\n===\n", next[0].str().c_str());
			}
		}
	}
}

void EventNodeBox::get_xml_data(pugi::xml_node& node) {
	node.append_attribute("guid").set_value(guid().str().c_str());
	node.append_attribute("amt_next").set_value(next.size());

	node.append_attribute("x").set_value(rect_.x);
	node.append_attribute("y").set_value(rect_.y);
	node.append_attribute("w").set_value(rect_.w);
	node.append_attribute("h").set_value(rect_.h);

	for (int i = 0; i < next.size(); ++i) {
		std::stringstream str;
		str << "next_" << i;
		node.append_attribute(str.str().c_str()).set_value(next[i].str().c_str());
	}
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


// TRIGGER //

TriggerBox::TriggerBox(bool interactable, bool is_in_place, bool activate_once) : EventNodeBox(1) {
	title = "Entry";
	has_in_ = false;

	interactable_ = std::make_unique<ToggleBox>("Inter", interactable);
	is_in_place_ = std::make_unique<ToggleBox>("In Place", is_in_place);
	activate_once_ = std::make_unique<ToggleBox>("Once", activate_once);
	on_box_moved();
}

TriggerBox::TriggerBox(pugi::xml_node& const node) : EventNodeBox(1, &node) {

	title = "Entry";
	has_in_ = false;

	interactable_ = std::make_unique<ToggleBox>("Inter", node.attribute("interactable").as_bool());
	is_in_place_ = std::make_unique<ToggleBox>("In Place", node.attribute("in_place").as_bool());
	activate_once_ = std::make_unique<ToggleBox>("Once", node.attribute("once").as_bool());
	on_box_moved();
}

void TriggerBox::draw(Game* game) {

	EventNodeBox::draw(game);

	interactable_->draw(game);
	is_in_place_->draw(game);
	activate_once_->draw(game);

}

void TriggerBox::handle_events(Game* game, const SDL_Event& e) {
	interactable_->handle_events(game, e);
	is_in_place_->handle_events(game, e);
	activate_once_->handle_events(game, e);

	EventNodeBox::handle_events(game, e);

}

void TriggerBox::get_xml_data(pugi::xml_node& node) {
	EventNodeBox::get_xml_data(node);
	node.append_attribute("type").set_value("trigger");
	node.append_attribute("interactable").set_value(interactable());
	node.append_attribute("in_place").set_value(is_in_place());
	node.append_attribute("once").set_value(activate_once());
}

void TriggerBox::on_box_moved() {

	SDL_Point pos = { rect_.x + 8, rect_.y + (rect_.h / 4) };
	interactable_->set_position(pos);
	pos.y += (rect_.h / 4);
	is_in_place_->set_position(pos);
	pos.y += (rect_.h / 4);
	activate_once_->set_position(pos);

}


// DIALOG NODE //

void DialogNodeBox::draw(Game* game) {

	EventNodeBox::draw(game);

	// Draw text box
	if (!is_editing_text)
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

void DialogNodeBox::get_xml_data(pugi::xml_node& node) {

	EventNodeBox::get_xml_data(node);
	node.append_attribute("type").set_value("dialog");
	node.append_attribute("dialog").set_value(text.c_str());

}

