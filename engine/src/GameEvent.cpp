#include "GameEvent.h"

#include <stdlib.h>
#include <time.h>
#include <memory>

#include "Components.h"
#include "DialogState.h"
#include "EventChain.h"
#include "Game.h"
#include "GameState.h"

namespace sl2dge {

std::unique_ptr<GameEvent> GameEvent::create_event(const pugi::xml_node &node) {
	std::string type = node.attribute("type").as_string();

	if (type == "dialog") {
		return std::move(std::make_unique<Dialog>(node));
	} else {
		SDL_Log("Type %s unknown", type);
		throw std::exception("Type unknown");
	}
}

GameEvent::GameEvent(const pugi::xml_node *node) {
	id_ = Guid(node->attribute("guid").as_string());
	rect_.x = node->attribute("x").as_int();
	rect_.y = node->attribute("y").as_int();
	rect_.w = node->attribute("w").as_int();
	rect_.h = node->attribute("h").as_int();
	resize(0, 0); // To avoid having boxes with 0 size
}

void GameEvent::get_xml_data(pugi::xml_node &node) {
	node.remove_attributes();
	node.append_attribute("guid").set_value(id_.str().c_str());
	node.append_attribute("x").set_value(rect_.x);
	node.append_attribute("y").set_value(rect_.y);
	node.append_attribute("w").set_value(rect_.w);
	node.append_attribute("h").set_value(rect_.h);
}

void GameEvent::draw(Game *game) {
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
		if (!has_prev) {
			SDL_RenderDrawRect(game->renderer(), &game->main_camera()->world_to_screen_transform(in_plug()));
		} else {
			SDL_RenderFillRect(game->renderer(), &game->main_camera()->world_to_screen_transform(in_plug()));
		}
	}

	// Out
	for (int i = 0; i < next_amt(); i++) {
		if (next(i).isNil()) {
			SDL_RenderDrawRect(game->renderer(), &game->main_camera()->world_to_screen_transform(out_plug(i)));
		} else {
			SDL_RenderFillRect(game->renderer(), &game->main_camera()->world_to_screen_transform(out_plug(i)));
		}
	}
}

void GameEvent::handle_events(Game *game, const SDL_Event &e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
		if (e.button.button == SDL_BUTTON_RIGHT) {
			if (SDL_PointInRect(&mouse_pos, rect())) {
				SDL_Log("===\nMe : %s", id().str().c_str());
				SDL_Log("Next : %s\n===\n", next().str().c_str());
			}
		}
	}
}

SDL_Rect GameEvent::in_plug() {
	if (has_in_)
		return SDL_Rect{ rect_.x - 10, rect_.y + rect_.h / 2, 10, 10 };
	else
		throw std::runtime_error("No in");
}

SDL_Rect GameEvent::out_plug(int i) {
	if (next_amt() == 0) {
		throw std::runtime_error("No out");
	} else if (next_amt() == 1) {
		return SDL_Rect{ rect_.x + rect_.w, rect_.y + rect_.h / 2, 10, 10 };
	} else {
		return SDL_Rect{ rect_.x + rect_.w, rect_.y + (rect_.h / (next_amt() + 1) * (i + 1)), 10, 10 };
	}
}

int GameEvent::is_point_in_plug(SDL_Point *point) {
	for (int i = 0; i < next_amt(); i++) {
		if (SDL_PointInRect(point, &out_plug(i)))
			return i;
	}
	return -1;
}

/* DIALOG */
Dialog::Dialog(const pugi::xml_node &node) :
		GameEvent(&node) {
	text = node.attribute("text").as_string();
	next_ = Guid(node.attribute("next").as_string());
}

void Dialog::get_xml_data(pugi::xml_node &node) {
	GameEvent::get_xml_data(node);
	node.append_attribute("type").set_value("dialog");
	node.append_attribute("text").set_value(text.c_str());
	node.append_attribute("next").set_value(next_.str().c_str());
}

void Dialog::on_activate(Game *game, EventChain *gem) {
	game->push_state(std::make_unique<DialogState>(gem, this));
}

void Dialog::on_end(Game *game) {
	game->pop_state();
}

void Dialog::draw(Game *game) {
	GameEvent::draw(game);

	// Draw text box
	if (!is_editing_text)
		SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 255);
	else
		SDL_SetRenderDrawColor(game->renderer(), 255, 0, 0, 255);
	auto pos = game->main_camera()->world_to_screen_transform(get_text_box());
	SDL_RenderDrawRect(game->renderer(), &pos);

	FC_DrawBox(game->font(), game->renderer(), pos, text.c_str());
}

void Dialog::handle_events(Game *game, const SDL_Event &e) {
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
}

// CHOICE
DialogChoice::DialogChoice(const pugi::xml_node &node) :
		Dialog(node) {
	choice_amt_ = node.attribute("choice_amt").as_int();
	for (int i = 0; i < choice_amt_; ++i) {
		std::stringstream ss;
		ss << "text_" << i;
		choices_.push_back(node.attribute(ss.str().c_str()).as_string());
		ss.clear();
		ss << "next_" << i;
		next_nodes_.push_back(Guid(node.attribute(ss.str().c_str()).as_string()));
	}
}

void DialogChoice::on_activate(Game *game, EventChain *gem) {
	game->push_state(std::make_unique<DialogState>(gem, this));
}

void DialogChoice::on_end(Game *game) {
	game->pop_state();
}

void DialogChoice::draw(Game *game) {
	Dialog::draw(game);
}

void DialogChoice::handle_events(Game *game, const SDL_Event &e) {
	Dialog::handle_events(game, e);
}

/*
	// TELEPORT 

	Teleport::Teleport(const Guid id, IMoveable* obj, const Point& pos, const Guid next)
		: GameEvent(id, next), pos(pos), obj(obj) {
	}
	void Teleport::on_activate(Game* game, EventChain* gem) {
		obj->teleport_to(Vector2(float(pos.x * 16), float(pos.y * 16)));
		gem->next_event(game);
	}

	// SCENE CHANGE 

	void SceneChange::on_activate(Game* game, EventChain* gem) {
		// SceneManager::LoadScene(game, this->scene_name);
		game->change_state(std::move(state_));
		//gem->next_event();
	}

	// QUEST  

	void QuestStatus::on_activate(Game* game, EventChain* gem) {
		game->quest_manager()->change_quest_status(quest_id_, new_status_);
		SDL_Log("Quest %d has progressed to step %d!", quest_id_, new_status_);
		gem->next_event(game);
	}

	// CONDITIONAL QUEST 

	void QuestEQBranch::on_activate(Game* game, EventChain* gem) {
		auto status = game->quest_manager()->quest(quest_id_)->status;
		this->next_ = status == status_to_check_ ? next_if_true_ : next_if_false_;
		SDL_Log("Checking if Quest %d is at step %d. Quest is at %d, jumping to event %d.", quest_id_, status_to_check_, status, this->next_);
		gem->next_event(game);

	}

	// Inherited via GameEvent

	void QuestGTBranch::on_activate(Game* game, EventChain* gem) {
		auto status = game->quest_manager()->quest(quest_id_)->status;
		this->next_ = status >= status_to_check_ ? next_if_greater_ : next_if_lower_;
		SDL_Log("Checking if Quest %d is at least above step %d. Quest is at %d, jumping to event %d.", quest_id_, status_to_check_, status, this->next_);
		gem->next_event(game);
	}

	void RandomBranch::on_activate(Game* game, EventChain* gem) {

		srand(unsigned int(time(NULL))); // TODO : make a rng

		auto val = rand() % this->next_nodes_.size();
		this->next_ = next_nodes_[val];
		gem->next_event(game);

	}

	void RandomBranch::on_end(Game* game) {

	}


	*/
} // namespace sl2dge
