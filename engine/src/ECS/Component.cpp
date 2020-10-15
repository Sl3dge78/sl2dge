#include "Component.h"

#include "ECS/Entity.h"

#include "scene/Camera.h"
#include "scene/map/TileMap.h"
#include "scene/physics/Rigibody.h"
#include "ui/UIPanel.h"
#include "ui/UIText.h"

namespace sl2dge {

Transform *Component::transform() {
	if (transform_ == nullptr)
		transform_ = entity()->transform();

	return transform_;
}
/* STATIC */

void Component::register_components() {
	register_component<Transform>("Transform");
	register_component<Sprite>("Sprite");
	register_component<SpriteAnimator>("SpriteAnimator");
	register_component<TileMap>("TileMap");
	register_component<CollisionMap>("CollisionMap");
	register_component<Camera>("Camera");
	register_component<Rigidbody>("Rigidbody");
	register_component<UIPanel>("UIPanel");
	register_component<UIText>("UIText");
	register_component<UIButton>("UIButton");
	register_component<UIContextMenu>("UIContextMenu");
	register_component<UIContextMenu>("UINumberField");
}

Component::component_map Component::component_list;
std::map<int, std::string> Component::component_names;
int Component::comp_id = 0;

Component *Component::create_component(const std::string &type) {
	if (component_list.find(type) == component_list.end()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Component type %s not registered!", type.c_str());
		throw std::runtime_error("Component type unknown!");
	}
	return component_list[type]();
}

const int Component::get_id(const std::string &name) {
	for (auto it = component_names.begin(); it != component_names.end(); ++it) {
		if (it->second == name)
			return it->first;
	}
	SDL_LogError(0, "Unable to find id of component %s", name.c_str());
	return -1;
}

} // namespace sl2dge
