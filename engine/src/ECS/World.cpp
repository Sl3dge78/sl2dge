#include "World.h"
#include <scene\Transform.h>

namespace sl2dge {

World::~World() {
	delete_all_systems();
	delete_all_entities();
}

// == ENTITIES ==

Entity *World::create_entity() {
	Entity *e = new Entity();
	entity_list_.push_back(e);
	e->add_component<Transform>();

	is_systems_entities_list_dirty_ = true;

	return e;
}

Entity *World::create_entity(const Vector2f &position) {
	Entity *e = new Entity();
	entity_list_.push_back(e);
	e->add_component<Transform>(position);

	is_systems_entities_list_dirty_ = true;

	return e;
}

void World::delete_all_entities() {
	for (Entity *e : entity_list_) {
		delete e;
	}

	is_systems_entities_list_dirty_ = true;

	entity_list_.clear();
}

void World::delete_entity(Entity *e) {
	for (auto it = entity_list_.begin(); it < entity_list_.end(); ++it) {
		if (*it == e) {
			for (auto child : e->get_children()) {
				this->delete_entity(child);
			}
			delete e;
			entity_list_.erase(it);
			is_systems_entities_list_dirty_ = true;
			return;
		}
	}
}

// == SYSTEMS ==

ISystem *World::add_system(ISystem *sys) {
	if (dynamic_cast<InitSystem *>(sys) != nullptr) {
		init_systems_.push_front(dynamic_cast<InitSystem *>(sys));
	}

	if (dynamic_cast<InputSystem *>(sys) != nullptr) {
		input_systems_.push_front(dynamic_cast<InputSystem *>(sys));
	}

	if (dynamic_cast<UpdateSystem *>(sys) != nullptr) {
		update_systems_.push_front(dynamic_cast<UpdateSystem *>(sys));
	}

	if (dynamic_cast<DrawSystem *>(sys) != nullptr) {
		draw_systems_.push_front(dynamic_cast<DrawSystem *>(sys));
		dynamic_cast<DrawSystem *>(sys)->renderer_ = this->renderer_;
		dynamic_cast<DrawSystem *>(sys)->font_ = this->main_font_;
		dynamic_cast<DrawSystem *>(sys)->camera_ = this->camera_->get_component<Camera>();

		draw_systems_.sort([](const DrawSystem *sys1, const DrawSystem *sys2) {
			return sys1->pos_z < sys2->pos_z;
		});
	}

	if (dynamic_cast<WorldSetSystem *>(sys) != nullptr) {
		dynamic_cast<WorldSetSystem *>(sys)->world_ = this;
	}

	systems_.push_front(sys);
	return sys;
}

void World::delete_all_systems() {
	for (ISystem *sys : systems_) {
		delete sys;
	}
	systems_.clear();
	init_systems_.clear();
	input_systems_.clear();
	update_systems_.clear();
	draw_systems_.clear();
}

void World::start() {
	for (InitSystem *system : init_systems_) {
		system->start();
	}
}

void World::handle_events(SDL_Event const &e) {
	for (InputSystem *system : input_systems_) {
		system->handle_events(e);
	}
}

void World::update(const int delta_time) {
	for (UpdateSystem *system : update_systems_) {
		system->update(delta_time);
	}
}

void World::draw() {
	for (DrawSystem *system : draw_systems_) {
		system->draw();
	}
}

void World::update_systems_entities() {
	if (is_systems_entities_list_dirty_) {
		for (auto &system : systems_) { // for each system
			if (system->filter_.size() > 0) {
				system->entities_.clear(); // TODO : Optimize this

				for (auto entity : entity_list_) { // Compare each entity to the filter list
					if (entity->components_.empty() || !entity)
						continue;

					bool add = false;
					for (auto &id : system->filter_) {
						if (!entity->has_component(id)) {
							if (system->filter_type_ == FILTER_AND) {
								add = false;
								break;
							}
						} else {
							add = true;
							if (system->filter_type_ == FILTER_OR)
								break;
						}
					}
					if (add)
						system->entities_.push_front(entity);
				}
				system->on_entity_list_changed();
			}
		}
		is_systems_entities_list_dirty_ = false;
	}
}

} // namespace sl2dge
