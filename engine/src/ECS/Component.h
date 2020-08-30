#pragma once

#include <fstream>
#include <string>

#include <SDL/SDL.h>

#include "addons/pugixml.hpp"

namespace sl2dge {
class Entity;
class Game;

class Component {
	friend class Entity;

public:
	Component() = default;
	virtual ~Component() = default;

	virtual void load(const pugi::xml_node &node) = 0;
	virtual void save(pugi::xml_node &node) = 0;

	virtual void start(Game *game){};
	virtual void update(Game *game){};
	virtual void handle_events(Game *game, SDL_Event const &e){};
	virtual void draw(Game *game){};

	Entity *entity() { return entity_; }

private:
	Entity *entity_ = nullptr;
};

// Used by entities to refer to components with an unique id
struct ComponentID {
public:
	template <class T>
	static const int Get() {
		static int id = count++;
		return id;
	}

private:
	static int count;
};
} // namespace sl2dge
