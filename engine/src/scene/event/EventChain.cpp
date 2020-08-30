#include "EventChain.h"

namespace sl2dge {

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

void EventChain::next_event(Game *game) {
	if (current_event != nullptr) {
		current_event->on_end(game);
		next = current_event->next();
	}

	current_event = nullptr;

	if (!next.isNil()) {
		current_event = get_event(next);
		current_event->on_activate(game, this);
		SDL_Log("Event %s triggered", next);
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

void EventChain::update(Game *game) {
	if (queue_next_event) {
		next_event(game);
		queue_next_event = false;
	}

	if (entity()->transform()->tiled_position() == player_->transform()->tiled_position()) {
		if (in_place && !interactable) {
			activate_chain(game, this);
		}
	}
}

void EventChain::handle_events(Game *game, SDL_Event const &e) {
	// TODO : Change that
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

EventChain *EventChain::get_chain_at(const Vector2i &pos) {
	// TODO : IMPLEMENT
	return nullptr;
}

void EventChain::activate_chain(Game *game, EventChain *chain) {
	if (!chain->entered && !chain->activated) {
		if (!chain->interactable) {
			chain->entered = true;
		}

		if (chain->activate_once) {
			chain->activated = true;
		}
		chain->queue_next_event = true;
	}
}

void EventChain::load(const pugi::xml_node &node) {
	interactable = node.attribute("interactable").as_bool();
	in_place = node.attribute("in_place").as_bool();
	activate_once = node.attribute("once").as_bool();
	next = Guid(node.attribute("next").as_string());

	for (auto event : node.children("Event")) {
		this->events_.push_back(GameEvent::create_event(event));
	}
}

} // namespace sl2dge
