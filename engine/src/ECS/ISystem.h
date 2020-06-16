#pragma once

#include "Entity.h"
#include <forward_list>
#include <vector>

namespace ECS {

enum FilterType { FILTER_AND,
	FILTER_OR };

class ISystem {
	friend class World;

private:
	void AddComponentFilter(const int id);

public:
	virtual ~ISystem() = default;
	const void GetActiveEntities(std::vector<Entity *> *list);
	void AddEntity(Entity *e);
	template <class T>
	void AddComponentFilter();
	void SetFilterType(FilterType type) { filter_type_ = type; };

protected:
	ISystem() = default;
	bool is_active_ = true;

	std::forward_list<Entity *> entities_;
	std::vector<int> filter_;
	FilterType filter_type_ = FILTER_AND;

	virtual void OnEntityAdded(Entity *entity){};
	virtual void OnEntityListChanged(){};
};

template <class T>
void ISystem::AddComponentFilter() {
	AddComponentFilter(ComponentID::Get<T>());
}

// == CHILDS ==

/// Update gets called every frame
class UpdateSystem {
protected:
	UpdateSystem() = default;

public:
	virtual ~UpdateSystem() = default;
	virtual void Update() = 0;
};

/// Draw gets called every frame after all logic
class DrawSystem {
	friend class World;

protected:
	DrawSystem() = default;
	ALLEGRO_FONT *font_;
	ALLEGRO_BITMAP *bitmap_;
	int pos_z = 0;

public:
	virtual ~DrawSystem() = default;
	virtual void Draw() = 0;
};

/// Input gets called when an event gets registered
class InputSystem {
protected:
	InputSystem() = default;

public:
	virtual ~InputSystem() = default;
	virtual void Input(ALLEGRO_EVENT *const ev) = 0;
};

//TODO : Add a reactive system that calls update/draw only when needed

//INTERFACES

/// Init gets called before the first loop starts
class InitSystem {
protected:
	InitSystem() = default;

public:
	virtual ~InitSystem() = default;
	virtual void Init() = 0;
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

} // namespace ECS
