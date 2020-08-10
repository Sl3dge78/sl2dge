#pragma once
#include <map>
#include <string>

#include "ECS/ECS.h"
#include "addons/pugixml.hpp"

namespace sl2dge {

class ISystem;
class Component;

typedef std::map<std::string, Component *(*)(Entity *entity, pugi::xml_node &)> component_map;
typedef std::map<std::string, ISystem *(*)(pugi::xml_node &)> system_map;

class ECS_DB {
public:
	static void register_all_systems();
	static void register_all_components();

	static ISystem *create_system(const std::string &type, pugi::xml_node &node);
	static Component *create_component(const std::string &type, Entity *entity, pugi::xml_node &node);

private:
	/* SYSTEMS */

	template <class T>
	static void register_system(std::string name) {
		system_list[name] = &make_system<T>;
	}

	template <class T>
	static ISystem *make_system(pugi::xml_node &node) {
		return new T(node);
	}

	static system_map system_list;

	/* COMPONENTS */

	template <class T>
	static void register_component(std::string name) {
		component_list[name] = &make_component<T>;
	}

	template <class T>
	static Component *make_component(Entity *entity, pugi::xml_node &node) {
		return entity->load_component<T>(node);
	}

	static component_map component_list;
};

} // namespace sl2dge
