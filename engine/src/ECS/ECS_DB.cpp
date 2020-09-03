#include "ECS/ECS_DB.h"

#include "scene/Camera.h"
#include "scene/map/TileMap.h"
#include "scene/physics/Rigibody.h"
#include "ui/UIPanel.h"
#include "ui/UIText.h"

using namespace sl2dge;

component_map ECS_DB::component_list;
std::map<int, std::string> ECS_DB::component_names;
int ECS_DB::comp_id = 0;

ECS_DB::ECS_DB() {
	register_component<Transform>("Transform");
	register_component<TileMap>("TileMap");
	register_component<CollisionMap>("CollisionMap");
	register_component<Camera>("Camera");
	register_component<Rigidbody>("Rigidbody");
	register_component<UIPanel>("UIPanel");
	register_component<UIText>("UIText");
}

ECS_DB::~ECS_DB() {
	ECS_DB::component_list.clear();
	ECS_DB::component_names.clear();
}

Component *ECS_DB::create_component(const std::string &type, pugi::xml_node &node) {
	if (component_list.find(type) == component_list.end()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Component type %s not registered!", type.c_str());
		throw std::runtime_error("Component type unknown!");
	}
	return component_list[type](node);
}

const int sl2dge::ECS_DB::get_component_id(const std::string &name) {
	for (auto it = component_names.begin(); it != component_names.end(); ++it) {
		if (it->second == name)
			return it->first;
	}
	SDL_LogError(0, "Unable to find id of component %s", name.c_str());
	return -1;
}
