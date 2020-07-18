#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Component.h"
#include "math/Vector.h"
#include "scene/Transform.h"

namespace sl2dge {
class World;

class Entity {
	friend class World;

private:
	std::map<int, Component *> components_;

public:
	Entity(const Vector2f &position = Vector2f(0, 0)) {
		transform_ = this->add_component<Transform>(position);
	}
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

	Transform *transform() {
		return transform_;
	};

protected:
	Component *add_component(const int id, Component *comp);
	Component *get_component(const int id) const;
	bool has_component(const int id) const;
	void remove_component(const int id);
	bool is_active_ = true;

	Transform *transform_ = nullptr;
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
