#include "ECS/ECS_DB.h"

#include "scene/Camera.h"
#include "scene/map/TileMap.h"
#include "scene/physics/Rigibody.h"

using namespace sl2dge;

component_map ECS_DB::component_list;
system_map ECS_DB::system_list;

void ECS_DB::register_all_systems() {
	register_system<TileMapSystem>("TileMapSystem");
	register_system<CameraSystem>("CameraSystem");
	register_system<PhysicsSystem>("PhysicsSystem");
}

void ECS_DB::register_all_components() {
	register_component<TileMap>("TileMap");
	register_component<Camera>("Camera");
	register_component<Rigidbody>("Rigidbody");
}

ISystem *ECS_DB::create_system(const std::string &type, pugi::xml_node &node) {
	if (system_list.find(type) == system_list.end()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "System type %s not registered!", type.c_str());
		throw std::runtime_error("System type unknown!");
	}
	return system_list[type](node);
}

Component *ECS_DB::create_component(const std::string &type, Entity *entity, pugi::xml_node &node) {
	if (component_list.find(type) == component_list.end()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Component type %s not registered!", type.c_str());
		throw std::runtime_error("Component type unknown!");
	}
	return component_list[type](entity, node);
}
