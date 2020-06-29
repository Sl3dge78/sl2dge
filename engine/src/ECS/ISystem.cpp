#include "ISystem.h"

namespace sl2dge::ECS {

void ISystem::add_entity(Entity *e) {
	entities_.push_front(e);
	on_entity_added(e);
	on_entity_list_changed();
}

//Go through all entities, and give only the ones that are activated
void const ISystem::get_active_entities(std::vector<Entity *> *list) {
	for (Entity *e : entities_) {
		if (e->get_is_active())
			list->push_back(e);
	}
}

void ISystem::add_component_filter(const int id) {
	filter_.push_back(id);
}

} // namespace sl2dge::ECS
