#pragma once
#include <map>
#include <string>

#include "addons/pugixml.hpp"

namespace sl2dge {

class Component;

typedef std::map<std::string, Component *(*)(pugi::xml_node &)> component_map;

class ECS_DB {
public:
	ECS_DB();
	~ECS_DB();

	static Component *create_component(const std::string &type, pugi::xml_node &node);

	template <class T>
	static const int get_component_id();

	static const int get_component_id(const std::string &name);

	template <class T>
	static const std::string get_component_string();

	static const std::string get_component_string(const int id) {
		return component_names[id];
	}

private:
	/* COMPONENTS */
	static int comp_id;

	template <class T>
	static void register_component(std::string name);

	template <class T>
	static Component *load_component(pugi::xml_node &node);

	static component_map component_list;
	static std::map<int, std::string> component_names;
};

template <class T>
inline const int ECS_DB::get_component_id() {
	static int id = comp_id++;
	return id;
}

template <class T>
inline const std::string ECS_DB::get_component_string() {
	auto a = component_names[get_component_id<T>()];
	return a;
}

template <class T>
inline void ECS_DB::register_component(std::string name) {
	component_list[name] = &load_component<T>;
	component_names[get_component_id<T>()] = name;
}

template <class T>
inline Component *ECS_DB::load_component(pugi::xml_node &node) {
	auto c = new T();
	c->load(node);
	return c;
}

} // namespace sl2dge
