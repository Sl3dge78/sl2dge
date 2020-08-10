#include "World.h"
#include "ECS_DB.h"
#include "scene\Transform.h"

namespace sl2dge {

// == ENTITIES ==

Entity *World::create_entity(const Vector2f &position) {
	std::unique_ptr<Entity> e = std::make_unique<Entity>(position);
	auto ret = e.get();
	entity_list_.push_back(std::move(e));
	is_systems_entities_list_dirty_ = true;

	return ret;
}

Entity *World::create_entity(const float x, const float y) {
	return create_entity(Vector2f(x, y));
}

void World::delete_all_entities() {
	is_systems_entities_list_dirty_ = true;
	entity_list_.clear();
}

void World::delete_entity(Entity *e) {
	for (auto it = entity_list_.begin(); it < entity_list_.end(); ++it) {
		if (it->get() == e) {
			for (auto child : e->transform()->get_children()) {
				this->delete_entity(child->entity());
			}
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

		draw_systems_.sort([](const DrawSystem *sys1, const DrawSystem *sys2) {
			return sys1->pos_z() < sys2->pos_z();
		});
	}

	if (dynamic_cast<WorldSetSystem *>(sys) != nullptr) {
		dynamic_cast<WorldSetSystem *>(sys)->world_ = this;
	}

	systems_.push_front(std::unique_ptr<ISystem>(sys));
	return sys;
}

void World::delete_all_systems() {
	init_systems_.clear();
	input_systems_.clear();
	update_systems_.clear();
	draw_systems_.clear();
	systems_.clear();
}

void World::start(Game *game) {
	for (InitSystem *system : init_systems_) {
		system->start(game);
	}
}

void World::handle_events(Game *game, SDL_Event const &e) {
	for (InputSystem *system : input_systems_) {
		system->handle_events(game, e);
	}
}

void World::update(Game *game) {
	for (UpdateSystem *system : update_systems_) {
		system->update(game);
	}
}

void World::draw(Game *game) {
	for (DrawSystem *system : draw_systems_) {
		system->draw(game);
	}
}

void World::update_systems_entities() {
	if (is_systems_entities_list_dirty_) {
		for (auto &system : systems_) { // for each system
			if (system->filter_.size() > 0) {
				system->entities_.clear(); // TODO : Optimize this

				for (auto &entity : entity_list_) {
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
						system->entities_.push_front(entity.get());
				}
				system->on_entity_list_changed();
			}
		}
		is_systems_entities_list_dirty_ = false;
	}
}

template <class T, class... Args>
T *World::create_system(Args &&... args) {
	return static_cast<T *>(add_system(new T(std::forward<Args>(args)...)));
}
} // namespace sl2dge
