#include "EventManagers.h"

#include "Game.h"
#include "DialogState.h"

namespace sl2dge {

#pragma region GAMEEVENT MANAGER

	GameEventManager::GameEventManager(Game* game) {

		this->game = game;

		auto s = IMG_Load(box_path.c_str());
		dialog_box_ = SDL_CreateTextureFromSurface(game->renderer(), s);
		SDL_FreeSurface(s);

		s = IMG_Load(selection_icon_path.c_str());
		selection_texture_ = SDL_CreateTextureFromSurface(game->renderer(), s);
		SDL_FreeSurface(s);

	}

	GameEventManager::~GameEventManager() {

		triggers.clear();
		event_list.clear();

		SDL_DestroyTexture(dialog_box_);
		SDL_DestroyTexture(selection_texture_);
	}

	void GameEventManager::add_game_event(GameEvent* e) {
		event_list.push_back(std::unique_ptr<GameEvent>(e));
	}

	GameEvent* GameEventManager::find_game_event(const int id) {
		for (auto&& x : event_list) {
			if (x->GetID() == id)
				return x.get();
		}
		return nullptr;
	}

	bool GameEventManager::trigger_event(const int id) {
		if (current_event != nullptr)
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Triggering Event id %d but there already is an event playing (id %d)", id, current_event->GetID());

		for (auto&& x : event_list) {
			if (x->GetID() == id) {
				current_event = x.get();
				current_event->on_activate(game, this);
				return true;
			}
		}
		return false;
	}

	bool GameEventManager::next_event() {

		current_event->on_end(game);
		auto next = current_event->GetNext();
		current_event = nullptr;

		if (next == -1) {
			
			return false;
		}
		return trigger_event(next);
	}

	void GameEventManager::add_trigger(Trigger* trigger) {
		triggers.push_back(std::unique_ptr<Trigger>(trigger));
	}

	void GameEventManager::update(const Point& player_position) {
		for (auto it = triggers.begin(); it != triggers.end(); it++) {
			if (!it->get()->is_interactable() && it->get()->position() == player_position) {
				activate_trigger(it->get());
				return;
			} else {
				it->get()->reset();
			}
		}
	}

	void GameEventManager::on_interact(const Point& player_position, Direction direction) {
		for (auto& trig : triggers) {
			if (trig->is_interactable()) {
				if (trig->is_in_place()) {
					if (trig->position() == player_position) {
						activate_trigger(trig.get());
						return;
					}
				} else {
					auto target_position = player_position;
					if (direction == Direction::Up) {
						target_position.y += -1;
					} else if (direction == Direction::Down) {
						target_position.y += 1;
					} else if (direction == Direction::Left) {
						target_position.x += -1;
					} else if (direction == Direction::Right) {
						target_position.x += 1;
					}
					if (trig->position() == target_position) {
						activate_trigger(trig.get());
						return;
					}
				}
			}
		}
	}
	void GameEventManager::activate_trigger(Trigger* trigger) {
		int next = trigger->on_trigger_activate();
		if (next == -1)
			return;

		trigger_event(next);
		return;
	}
}