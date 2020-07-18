#pragma once

#include "ECS/ECS.h"
#include "addons/pugixml.hpp"
#include "scene/Transform.h"
#include "scene/event/GameEvent.h"

namespace sl2dge {

class EventChain : public Component {
public:
	EventChain(const pugi::xml_node &node);
	EventChain(){};
	~EventChain(){};

	void add_event(GameEvent *e);
	GameEvent *get_event(const Guid id);

	std::vector<std::unique_ptr<GameEvent>> *events() {
		return &events_;
	}

	Guid next = Guid();
	bool interactable = false; // you need to activate it
	bool in_place = true; //  you need to stand on the item to trigger the event
	bool activate_once = false; // If true, won't activate ever again

	bool entered = false;
	bool activated = false;

	void save(pugi::xml_node &node);

	bool next_event(Game *game);
	bool trigger_event(Game *game, const Guid &id);

private:
	std::vector<std::unique_ptr<GameEvent>> events_;

	GameEvent *current_event = nullptr;
};

class EventSystem : public ISystem, public UpdateSystem, public InputSystem {
public:
	EventSystem(Entity *player);
	void update(Game *game) override;
	void handle_events(Game *game, SDL_Event const &e) override;

private:
	Entity *player_;

	EventChain *get_chain_at(const Vector2i &pos);
	void activate_chain(Game *game, EventChain *chain);
};
} // namespace sl2dge
