//
//  Entity.cpp
//  AntiquityTrader
//
//  Created by Guillaume Collin on 17/12/2018.
//  Copyright © 2018 Sledge. All rights reserved.
//

#include "Entity.h"

namespace sl2dge::ECS {

Entity::~Entity() {
	this->Entity::remove_all_components();
	for (auto e : childs_) {
		e->parent_ = nullptr;
	}
	childs_.clear();
}

void Entity::remove_all_components() {
	for (auto pair : components_) {
		delete pair.second;
	}
	components_.clear();
}

Component *Entity::add_component(const int id, Component *comp) {
	components_[id] = comp;
	return comp;
}

bool Entity::has_component(const int id) const {
	if (components_.empty() || components_.size() == 0)
		return false;

	return (components_.find(id) != components_.end());
}

Component *Entity::get_component(const int id) const {
	if (has_component(id)) {
		return components_.at(id);
	} else
		return nullptr;
}

void Entity::remove_component(const int id) {
	if (has_component(id)) {
		delete components_[id];
		components_.erase(id);
	}
}

void Entity::set_is_active(bool val) {
	is_active_ = val;
	for (auto &e : childs_)
		e->set_is_active(val);
}

void Entity::remove_all_children() {
	for (auto &e : childs_) {
		e->parent_ = nullptr;
	}
	childs_.clear();
}

void Entity::remove_children(ECS::Entity *child) {
	for (auto it = childs_.begin(); it != childs_.end(); ++it) {
		if (*it == child) {
			(*it)->parent_ = nullptr;
			childs_.erase(it);
			return;
		}
	}
}
} // namespace sl2dge::ECS
