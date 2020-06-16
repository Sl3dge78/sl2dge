#include "EventManager.h"

#include "Game.h"
#include "DialogState.h"

/*
namespace sl2dge {

	GameEventManager::GameEventManager(Game* game, const pugi::xml_node& events_node) {
		
		this->game = game;
		

		if (events_node.child("Event_Chain")) {
			//We found events, iterate through them and add them to the list !
			for (auto event_chain : events_node.children("Event_Chain")) {
				Point position = { event_chain.attribute("x_pos").as_int(), event_chain.attribute("y_pos").as_int() };
				for (auto event : event_chain.children("Event")) {
					std::string type = event.attribute("type").as_string();
					if (type == "trigger") {
						triggers.push_back(std::make_unique<Trigger>(position, Guid(event.attribute("next_0").as_string()), event.attribute("interactable").as_bool(), event.attribute("in_place").as_bool(), event.attribute("once").as_bool()));
					} else if (type == "dialog") {
						event_list.push_back(std::make_unique<Dialog>(Guid(event.attribute("guid").as_string()), event.attribute("dialog").as_string(), Guid(event.attribute("next_0").as_string())));
					} else {
						SDL_Log("Event type %s not implemented", type.c_str());
					}
				}
			}
		}
	}

	GameEventManager::~GameEventManager() {

		triggers.clear();
		event_list.clear();

	}

	bool GameEventManager::trigger_event(const Guid id) {
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

		if (next.isNil()) {
			return false;
		}
		return trigger_event(next);
	}

	void GameEventManager::update(Player* player) {
		for (auto it = triggers.begin(); it != triggers.end(); it++) {
			if (!it->get()->is_interactable() && it->get()->position() == player->tiled_position()) {
				activate_trigger(it->get());
				return;
			} else {
				it->get()->reset();
			}
		}
	}

	void GameEventManager::on_interact(Player* player) {
		for (auto& trig : triggers) {
			if (trig->is_interactable()) {
				if (trig->is_in_place()) {
					if (trig->position() == player->tiled_position()) {
						activate_trigger(trig.get());
						return;
					}
				} else {
					auto target_position = player->tiled_position();
					if (player->facing_direction() == Direction::Up) {
						target_position.y += -1;
					} else if (player->facing_direction() == Direction::Down) {
						target_position.y += 1;
					} else if (player->facing_direction() == Direction::Left) {
						target_position.x += -1;
					} else if (player->facing_direction() == Direction::Right) {
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
	void GameEventManager::save(pugi::xml_node& node) {

	}

	void GameEventManager::activate_trigger(Trigger* trigger) {
		Guid next = trigger->on_trigger_activate();
		if (next.isNil())
			return;

		trigger_event(next);
		return;
	}
}
*/