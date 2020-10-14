#pragma once

#include <fstream>
#include <map>
#include <string>

#include <SDL/SDL.h>

#include "addons/pugixml.hpp"

#define COMPONENT_DFLT_BEHAVIOUR(TYPE)                \
public:                                               \
	const std::string type_name() const override {    \
		return Component::type_from_template<TYPE>(); \
	}                                                 \
	const int type_id() const override {              \
		return Component::get_id<TYPE>();             \
	}

namespace sl2dge {
class Game;
class Entity;
class Transform;

class Component {
	friend class Entity;

public:
	Component() = default;
	virtual ~Component() = default;

	virtual void load(const pugi::xml_node &node){};
	virtual void save(pugi::xml_node &node){};

	virtual void start(Game *game){};
	virtual void update(Game *game){};
	virtual void handle_events(Game *game, SDL_Event const &e){};
	virtual void draw(Game *game){};

	Entity *entity() { return entity_; }
	Transform *transform();

	virtual const std::string type_name() const = 0;
	virtual const int type_id() const = 0;

private:
	Entity *entity_ = nullptr;
	Transform *transform_ = nullptr;

	/* 
	* STATIC MEMBERS 
	*/
public:
	static void register_components();
	static Component *create_component(const std::string &type);

	template <class T>
	static const int get_id();
	static const int get_id(const std::string &name);

	template <class T>
	static const std::string type_from_template();
	static const std::string type_from_id(const int id) {
		return component_names[id];
	}

	static const size_t component_amount() {
		return component_list.size();
	}

private:
	static int comp_id;
	typedef std::map<std::string, Component *(*)()> component_map;
	static component_map component_list;
	static std::map<int, std::string> component_names;

	template <class T>
	static Component *load_component();

	template <class T>
	static void register_component(std::string name);
};

template <class T>
const int Component::get_id() {
	static int id = comp_id++;
	return id;
}

template <class T>
const std::string Component::type_from_template() {
	return component_names[get_id<T>()];
}

template <class T>
void Component::register_component(std::string name) {
	component_list[name] = &load_component<T>;
	component_names[get_id<T>()] = name;
}

template <class T>
inline Component *Component::load_component() {
	auto c = new T();
	return c;
}

} // namespace sl2dge
