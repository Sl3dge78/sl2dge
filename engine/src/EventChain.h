#pragma once

#include "GameEvent.h"
#include "xml/pugixml.hpp"

namespace sl2dge {

class Scene;

class EventChain {
public:
	EventChain(const pugi::xml_node &node);
	EventChain(const int x, const int y) :
			position_({ x, y }){};
	~EventChain(){};

	void add_event(GameEvent *e);
	GameEvent *get_event(const Guid id);

	std::vector<std::unique_ptr<GameEvent>> *events() {
		return &events_;
	}

	void activate(Game *game);
	bool next_event(Game *game);
	bool trigger_event(Game *game, const Guid &id);

	Guid next() { return next_; }
	bool interactable() const { return interactable_; }
	bool in_place() const { return in_place_; }
	bool once() const { return activate_once_; }
	bool activated() const { return entered_; }
	Point position() const { return position_; }

	void set_interactable(const bool a) { interactable_ = a; }
	void set_in_place(const bool a) { in_place_ = a; }
	void set_once(const bool a) { activate_once_ = a; }
	void set_next(const Guid id) { next_ = id; }

	void get_xml_data(pugi::xml_node &node);

private:
	std::vector<std::unique_ptr<GameEvent>> events_;

	GameEvent *current_event = nullptr;

	Guid next_ = Guid();
	bool interactable_ = false; // you need to activate it
	bool in_place_ = true; //  you need to stand on the item to trigger the event
	bool activate_once_ = false; // If true, won't activate ever again

	Point position_;
	bool entered_ = false;
	bool activated_ = false;
};
} // namespace sl2dge
