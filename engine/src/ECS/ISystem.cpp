#include "ISystem.h"

namespace ECS {

void ISystem::AddEntity(Entity *e) {
	entities_.push_front(e);
	OnEntityAdded(e);
	OnEntityListChanged();
}

//Go through all entities, and give only the ones that are activated
void const ISystem::GetActiveEntities(std::vector<Entity *> *list) {
	for (Entity *e : entities_) {
		if (e->GetIsActive())
			list->push_back(e);
	}
}

void ISystem::AddComponentFilter(const int id) {
	filter_.push_back(id);
}

} // namespace ECS
