#pragma once

#include <fstream>
#include <string>

#include <SDL/SDL.h>

#include "addons/pugixml.hpp"

namespace sl2dge {
class Entity;

class Component {
	friend class Entity;

public:
	Component() = default;
	virtual ~Component() = 0;
	Entity *entity() { return entity_; }

private:
	Entity *entity_;
};
inline Component::~Component() = default;

// Used by entities to refer to components with an unique id
struct ComponentID {
public:
	template <class T>
	static const int Get() {
		static int id = count++;
		return id;
	}

private:
	static int count;
};
} // namespace sl2dge
