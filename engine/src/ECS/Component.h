#pragma once

#include <fstream>
#include <map>
#include <string>

#include <SDL/SDL.h>

#include "addons/pugixml.hpp"

#define COMPONENT_DFLT_BEHAVIOUR(TYPE)             \
public:                                            \
	const std::string type_name() const override { \
		return Component::get_type_name<TYPE>();   \
	}                                              \
	const int type_id() const override {           \
		return Component::get_id<TYPE>();          \
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

	virtual void load(const pugi::xml_node &node) = 0;
	virtual void save(pugi::xml_node &node) = 0;

	virtual void start(Game *game) = 0;
	virtual void update(Game *game) = 0;
	virtual void handle_events(Game *game, SDL_Event const &e) = 0;
	virtual void draw(Game *game) = 0;

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
	static Component *create_component(const std::string &type, pugi::xml_node &node);

	template <class T>
	static const int get_id();

	static const int get_id(const std::string &name);

	template <class T>
	static const std::string get_type_name();

	static const std::string get_type_name(const int id) {
		return component_names[id];
	}

private:
	static int comp_id;
	typedef std::map<std::string, Component *(*)(pugi::xml_node &)> component_map;
	static component_map component_list;
	static std::map<int, std::string> component_names;

	template <class T>
	static Component *load_component(pugi::xml_node &node);

	template <class T>
	static void register_component(std::string name);
};

template <class T>
const int Component::get_id() {
	static int id = comp_id++;
	return id;
}

template <class T>
const std::string Component::get_type_name() {
	return component_names[get_id<T>()];
}

template <class T>
void Component::register_component(std::string name) {
	component_list[name] = &load_component<T>;
	component_names[get_id<T>()] = name;
}

template <class T>
inline Component *Component::load_component(pugi::xml_node &node) {
	auto c = new T();
	c->load(node);
	return c;
}

} // namespace sl2dge
