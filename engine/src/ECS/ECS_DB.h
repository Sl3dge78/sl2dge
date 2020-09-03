#pragma once
#include <map>
#include <string>

#include "ECS/ECS.h"
#include "addons/pugixml.hpp"

namespace sl2dge {

class ISystem;
class Component;

typedef std::map<std::string, Component *(*)(Entity *entity, pugi::xml_node &)> component_map;

class ECS_DB {
public:
	ECS_DB();
	~ECS_DB();

	static Component *create_component(const std::string &type, Entity *entity, pugi::xml_node &node);

private:
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
	static component_map component_names_list;
};

} // namespace sl2dge
