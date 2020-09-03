#include "Entity.h"

#include "ECS/Component.h"

namespace sl2dge {
Entity::Entity() {
}

Entity::~Entity() {
	this->Entity::remove_all_components();
}

void Entity::remove_all_components() {
	components_.clear();
}

const std::vector<int> Entity::all_components_id() {
	std::vector<int> v;

	for (const auto &i : components_) {
		v.push_back(i.first);
	}

	return v;
}

Component *Entity::add_component(Component *comp) {
	comp->entity_ = this;
	components_[comp->type_id()] = std::unique_ptr<Component>(comp);
	return comp;
}

bool Entity::has_component(const int id) const {
	if (components_.empty() || components_.size() == 0)
		return false;

	return (components_.find(id) != components_.end());
}

Component *Entity::get_component(const int id) const {
	if (has_component(id)) {
		return components_.at(id).get();
	} else {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Component id %s not found!", id);
		throw std::runtime_error("Component not found!");
	}
}

void Entity::remove_component(const int id) {
	if (has_component(id)) {
		components_.erase(id);
	}
}

void Entity::set_is_active(bool val) {
	is_active_ = val;
	for (auto &e : this->transform()->get_children()) {
		e->entity()->set_is_active(val);
	}
}

} // namespace sl2dge
