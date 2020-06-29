#pragma once

#include <forward_list>
#include <memory>
#include <vector>

#include "../SDL_FontCache.h"
#include <SDL/SDL.h>

#include "Entity.h"

namespace sl2dge::ECS {

enum FilterType { FILTER_AND,
	FILTER_OR };

class ISystem {
	friend class World;

private:
	void add_component_filter(const int id);

public:
	virtual ~ISystem() = default;
	const void get_active_entities(std::vector<Entity *> *list);
	void add_entity(Entity *e);
	template <class T>
	void add_component_filter();
	void set_filter_type(FilterType type) { filter_type_ = type; };

protected:
	ISystem() = default;
	bool is_active_ = true;

	std::forward_list<Entity *> entities_;
	std::vector<int> filter_;
	FilterType filter_type_ = FILTER_AND;

	virtual void on_entity_added(Entity *entity){};
	virtual void on_entity_list_changed(){};
};

template <class T>
void ISystem::add_component_filter() {
	add_component_filter(ComponentID::Get<T>());
}

// == CHILDS ==

/// Update gets called every frame
class UpdateSystem {
protected:
	UpdateSystem() = default;

public:
	virtual ~UpdateSystem() = default;
	virtual void update() = 0;
};

/// Draw gets called every frame after all logic
class DrawSystem {
	friend class World;

protected:
	DrawSystem() = default;
	FC_Font *font_;
	SDL_Renderer *renderer_;
	int pos_z = 0;

public:
	virtual ~DrawSystem() = default;
	virtual void draw() = 0;
};

/// Input gets called when an event gets registered
class InputSystem {
protected:
	InputSystem() = default;

public:
	virtual ~InputSystem() = default;
	virtual void input(SDL_Event const &e) = 0;
};

//TODO : Add a reactive system that calls update/draw only when needed

//INTERFACES

/// Init gets called before the first loop starts
class InitSystem {
protected:
	InitSystem() = default;

public:
	virtual ~InitSystem() = default;
	virtual void init() = 0;
};

/// A system that requires a reference to the world to work.
class WorldSetSystem {
	friend class World;

protected:
	WorldSetSystem() = default;
	World *world_;

public:
	virtual ~WorldSetSystem() = default;
};

} // namespace sl2dge::ECS
