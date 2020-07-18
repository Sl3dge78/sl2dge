#include "EventChain.h"

namespace sl2dge {
EventChain::EventChain(const pugi::xml_node &chain_node) {
	entity()->get_component<Transform>()->position.x = chain_node.attribute("x_pos").as_int() * 16;
	entity()->get_component<Transform>()->position.y = chain_node.attribute("y_pos").as_int() * 16;
	interactable = chain_node.attribute("interactable").as_bool();
	in_place = chain_node.attribute("in_place").as_bool();
	activate_once = chain_node.attribute("once").as_bool();
	next = Guid(chain_node.attribute("next").as_string());

	for (auto event : chain_node.children("Event")) {
		this->events_.push_back(GameEvent::create_event(event));
	}
}

void EventChain::add_event(GameEvent *e) {
	//events_.push_back(std::unique_ptr<GameEvent>(e));
}

GameEvent *EventChain::get_event(const Guid id) {
	if (id.isNil())
		return nullptr;

	for (auto &&x : events_) {
		if (x->id() == id)
			return x.get();
	}
	return nullptr;
}

bool EventChain::next_event(Game *game) {
	if (current_event != nullptr) {
		current_event->on_end(game);
		auto next = current_event->next();
		current_event = nullptr;
	}

	if (next.isNil()) {
		return false;
	}

	return trigger_event(game, next);
}

bool EventChain::trigger_event(Game *game, const Guid &id) {
	if (current_event != nullptr)
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Triggering Event id %d but there already is an event playing (id %d)", id, current_event->id());

	for (auto &&x : events_) {
		if (x->id() == id) {
			current_event = x.get();
			current_event->on_activate(game, this);
			return true;
		}
	}
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Event %d not found!", id);
	return false;
}

void EventSystem::activate_chain(Game *game, EventChain *chain) {
	if (!chain->entered && !chain->activated) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Event %d triggered", chain->next);
		if (!chain->interactable) {
			chain->entered = true;
		}

		if (chain->activate_once) {
			chain->activated = true;
		}
		chain->next_event(game);
	}
}

void EventChain::save(pugi::xml_node &chain_node) {
	chain_node.remove_attributes();
	auto position = entity()->get_component<Transform>()->tiled_position();
	chain_node.append_attribute("x_pos").set_value(position.x);
	chain_node.append_attribute("y_pos").set_value(position.y);
	chain_node.append_attribute("interactable").set_value(interactable);
	chain_node.append_attribute("in_place").set_value(in_place);
	chain_node.append_attribute("once").set_value(activate_once);
	chain_node.append_attribute("next").set_value(next.str().c_str());

	chain_node.remove_children();

	for (auto &&e : events_) {
		auto event_node = chain_node.append_child("Event");
		e->get_xml_data(event_node);
	}
}

EventSystem::EventSystem(Entity *player) :
		player_(player) {
	this->add_component_filter<EventChain>();
}

void EventSystem::update(Game *game) {
	// TODO : Call this only when the player has moved
	auto chain = get_chain_at(player_->get_component<Transform>()->tiled_position());
	if (chain != nullptr && chain->in_place && !chain->interactable) {
		activate_chain(game, chain);
	}
}

void EventSystem::handle_events(Game *game, SDL_Event const &e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		if (e.key.keysym.scancode == SDL_SCANCODE_E) {
			auto chain = get_chain_at(player_->get_component<Transform>()->tiled_position());
			if (chain != nullptr && chain->in_place && chain->interactable) {
				activate_chain(game, chain);
				return;
			}
			/*        
			auto target_position = player_->tiled_position();
			switch (player_->facing_direction()) {
				case Direction::Up:
					target_position.y += -1;
					break;
				case Direction::Down:
					target_position.y += 1;
					break;
				case Direction::Left:
					target_position.x += -1;
					break;
				case Direction::Right:
					target_position.x += 1;
					break;
			}

			chain = get_chain_at(target_position.x, target_position.y);
			if (chain != nullptr && !chain->in_place && chain->interactable) {
				activate_chain(*chain);
				return;
			}
				*/
		}
	}
}

EventChain *EventSystem::get_chain_at(const Vector2i &pos) {
	for (auto e : entities_) {
		auto chain = e->get_component<EventChain>();

		if (e->get_component<Transform>()->tiled_position() == pos)
			return chain;
	}
	return nullptr;
}

} // namespace sl2dge
