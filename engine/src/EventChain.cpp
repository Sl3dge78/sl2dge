#include "EventChain.h"

#include "Scene.h"

namespace sl2dge {
	EventChain::EventChain(const pugi::xml_node& chain_node) {
		//trigger_ = Trigger(chain_node);
		position_ = { chain_node.attribute("x_pos").as_int(), chain_node.attribute("y_pos").as_int() };
		interactable_ = chain_node.attribute("interactable").as_bool();
		in_place_ = chain_node.attribute("in_place").as_bool();
		activate_once_ = chain_node.attribute("once").as_bool();
		next_ = Guid(chain_node.attribute("next").as_string());

		for (auto event : chain_node.children("Event")) {
			this->events_.push_back(GameEvent::create_event(event));
		}
	}

	void EventChain::add_event(GameEvent* e) {
		events_.push_back(std::unique_ptr<GameEvent>(e));
	}

	GameEvent* EventChain::get_event(const Guid id) {
		if (id.isNil())
			return nullptr;

		for (auto&& x : events_) {
			if (x->id() == id)
				return x.get();
		}
		return nullptr;
	}

	void EventChain::activate(Game* game) {
		if (!entered_ && !activated_) {
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Event %d triggered", next_);
			if (!interactable_) {
				entered_ = true;
			}

			if (activate_once_) {
				activated_ = true;
			}
			trigger_event(game, next_);
		}
	}

	bool EventChain::next_event(Game* game) {
		current_event->on_end(game);
		auto next = current_event->next();
		current_event = nullptr;

		if (next.isNil()) {
			return false;
		}
		return trigger_event(game, next);
	}

	bool EventChain::trigger_event(Game* game, const Guid& id) {
		if (current_event != nullptr)
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Triggering Event id %d but there already is an event playing (id %d)", id, current_event->id());

		for (auto&& x : events_) {
			if (x->id() == id) {
				current_event = x.get();
				current_event->on_activate(game, this);
				return true;
			}
		}

		return false;
	}

	void EventChain::get_xml_data(pugi::xml_node& chain_node) {

		chain_node.remove_attributes();

		chain_node.append_attribute("x_pos").set_value(position_.x);
		chain_node.append_attribute("y_pos").set_value(position_.y);
		chain_node.append_attribute("interactable").set_value(interactable_);
		chain_node.append_attribute("in_place").set_value(in_place_);
		chain_node.append_attribute("once").set_value(activate_once_);
		chain_node.append_attribute("next").set_value(next_.str().c_str());

		chain_node.remove_children();

		for (auto&& e : events_) {
			auto event_node = chain_node.append_child("Event");
			e->get_xml_data(event_node);
		}
	}
}