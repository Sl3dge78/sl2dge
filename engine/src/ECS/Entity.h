#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Component.h"

namespace sl2dge {
class World;

class Entity {
	friend class World;

private:
	std::map<int, Component *> components_;

public:
	Entity() = default;
	~Entity();

	template <class T, class... Args>
	inline T *add_component(Args &&... args);
	template <class T>
	inline bool has_component() const;
	template <class T>
	inline T *get_component() const;

	template <class T>
	inline void remove_component();

	void remove_all_components();

	bool get_is_active() const { return is_active_; };
	void set_is_active(bool val);

	//Children management
	Entity *get_parent() const { return parent_; };
	std::vector<Entity *> get_children() const { return childs_; };
	void add_children(Entity *child) {
		childs_.push_back(child);
		child->parent_ = this;
	};
	void remove_children(Entity *child);
	void remove_all_children();
	int order_ = 0;

protected:
	Component *add_component(const int id, Component *comp);
	Component *get_component(const int id) const;
	bool has_component(const int id) const;
	void remove_component(const int id);
	bool is_active_ = true;

	Entity *parent_ = nullptr;
	std::vector<Entity *> childs_;
};

//The following functions are user side functions for adding, getting and removing specific components
template <class T, class... Args>
T *Entity::add_component(Args &&... args) {
	return static_cast<T *>(add_component(ComponentID::Get<T>(), new T{ std::forward<Args>(args)... }));
}

template <class T>
bool Entity::has_component() const {
	return has_component(ComponentID::Get<T>());
}

template <class T>
T *Entity::get_component() const {
	return static_cast<T *>(get_component(ComponentID::Get<T>()));
}

//Removes a component from an entity
template <class T>
void Entity::remove_component() {
	remove_component(ComponentID::Get<T>());
}

} // namespace sl2dge
