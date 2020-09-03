#include "ECS/ECS_DB.h"

#include "scene/Camera.h"
#include "scene/map/TileMap.h"
#include "scene/physics/Rigibody.h"
#include "ui/UIPanel.h"

using namespace sl2dge;

component_map ECS_DB::component_list;

ECS_DB::ECS_DB() {
	register_component<TileMap>("TileMap");
	register_component<CollisionMap>("CollisionMap");
	register_component<Camera>("Camera");
	register_component<Rigidbody>("Rigidbody");
	register_component<Transform>("Transform");
	register_component<UIPanel>("UIPanel");
}

ECS_DB::~ECS_DB() {
	ECS_DB::component_list.clear();
}

Component *ECS_DB::create_component(const std::string &type, Entity *entity, pugi::xml_node &node) {
	if (component_list.find(type) == component_list.end()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Component type %s not registered!", type.c_str());
		throw std::runtime_error("Component type unknown!");
	}
	return component_list[type](entity, node);
}
