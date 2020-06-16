//
//  Entity.cpp
//  AntiquityTrader
//
//  Created by Guillaume Collin on 17/12/2018.
//  Copyright © 2018 Sledge. All rights reserved.
//

#include "Entity.h"

namespace ECS {

	Entity::~Entity() {
		this->Entity::RemoveAllComponents();
		for (auto e : childs_) {
			e->parent_ = nullptr;
		}
		childs_.clear();
	}

	void Entity::RemoveAllComponents() {
		for (auto pair : components_) {
			delete pair.second;
		}
		components_.clear();
	}

	Component* Entity::AddComponent(const int id, Component* comp) {
		components_[id] = comp;
		return comp;
	}

	bool Entity::HasComponent(const int id) const {
		if (components_.empty() || components_.size() == 0)
			return false;

		return (components_.find(id) != components_.end());
	}

	Component* Entity::GetComponent(const int id) const {
		if (HasComponent(id)) {
			return components_.at(id);
		} else
			return nullptr;
	}

	void Entity::RemoveComponent(const int id) {
		if (HasComponent(id)) {
			delete components_[id];
			components_.erase(id);
		}
	}

	void Entity::SetIsActive(bool val) {
		is_active_ = val;
		for (auto& e : childs_)
			e->SetIsActive(val);
	}

	void Entity::RemoveAllChildren() {
		for (auto& e : childs_) {
			e->parent_ = nullptr;
		}
		childs_.clear();
	}


	void Entity::RemoveChildren(ECS::Entity* child) {
		for (auto it = childs_.begin(); it != childs_.end(); ++it) {
			if (*it == child) {
				(*it)->parent_ = nullptr;
				childs_.erase(it);
				return;
			}
		}
	}
}
