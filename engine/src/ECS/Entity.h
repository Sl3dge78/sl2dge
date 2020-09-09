#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "math/Vector.h"
#include "scene/Transform.h"

namespace sl2dge {
class World;
class Game;
class Component;

class Entity {
	friend class World;

private:
	std::map<int, std::unique_ptr<Component>> components_;

public:
	Entity(World *world);
	~Entity();

	template <class T, class... Args>
	inline T *add_component(Args &&...args);

	Component *add_component_from_id(int id);

	template <class T>
	Component *load_component(pugi::xml_node &node);

	template <class T>
	inline bool has_component() const;
	template <class T>
	inline T *get_component() const;

	template <class T>
	inline void remove_component();

	void remove_component(Component *component);

	const std::vector<int> all_components_id();
	const std::vector<Component *> all_components();
	void remove_all_components();

	bool get_is_active() const { return is_active_; };
	void set_is_active(bool val);

	Transform *transform() {
		if (transform_ == nullptr)
			transform_ = this->get_component<Transform>();

		return transform_;
	};

	World *world() {
		return world_;
	}

	bool to_delete = false;

protected:
	Component *add_component(Component *comp);
	Component *get_component(const int id) const;
	bool has_component(const int id) const;
	void remove_component(const int id);
	bool is_active_ = true;

	Transform *transform_ = nullptr;
	World *world_ = nullptr;
};

//The following functions are user side functions for adding, getting and removing specific components
template <class T, class... Args>
T *Entity::add_component(Args &&...args) {
	return static_cast<T *>(add_component(new T{ std::forward<Args>(args)... }));
}

template <class T>
Component *Entity::load_component(pugi::xml_node &node) {
	auto comp = add_component(new T());
	comp->load(node);
	return comp;
}

template <class T>
bool Entity::has_component() const {
	return has_component(Component::get_id<T>());
}

template <class T>
T *Entity::get_component() const {
	return static_cast<T *>(get_component(Component::get_id<T>()));
}

//Removes a component from an entity
template <class T>
void Entity::remove_component() {
	remove_component(Component::get_id<T>());
}
} // namespace sl2dge
