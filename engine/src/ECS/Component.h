#pragma once

#include <fstream>
#include <string>

#include <SDL/SDL.h>

#include "ECS/ECS_DB.h"

#include "addons/pugixml.hpp"

namespace sl2dge {
class Game;
class Entity;
class Transform;

class Component {
	friend class Entity;

public:
	Component() = default;
	virtual ~Component() = default;

	virtual void load(const pugi::xml_node &node) = 0;
	virtual void save(pugi::xml_node &node) = 0;

	virtual void start(Game *game) = 0;
	virtual void update(Game *game) = 0;
	virtual void handle_events(Game *game, SDL_Event const &e) = 0;
	virtual void draw(Game *game) = 0;

	Entity *entity() { return entity_; }
	Transform *transform();

	std::string to_string() {
		return _to_string(this);
	}

private:
	Entity *entity_ = nullptr;
	Transform *transform_ = nullptr;

	template <class T>
	std::string _to_string(T comp);
};

template <class T>
std::string Component::_to_string(T comp) {
	auto a = ECS_DB::get_component_string<T>();
	return a;
}

} // namespace sl2dge
