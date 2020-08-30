#include "World.h"
#include "ECS_DB.h"

namespace sl2dge {

// == ENTITIES ==

World::World(const std::string &path) {
	load(path);
}

void World::load(const std::string &path) {
	SDL_Log("Loading scene : %s", path.c_str());
	pugi::xml_document doc;
	open_xml_doc(&doc, path);
	SDL_Log("=====");
	SDL_Log("Loading entities...");
	int count = 0;
	for (auto entity_node : doc.children("Entity")) {
		auto entity = this->create_entity();
		SDL_Log("> Entity added");

		for (auto component_node : entity_node.children("Component")) {
			// Add component
			std::string type = component_node.attribute("type").as_string();
			auto comp = ECS_DB::create_component(type, entity, component_node);
			SDL_Log(">> Component of type %s added", type.c_str());
		}
		count++;
	}
	SDL_Log("%d entities loaded!", count);
}

void World::save(const std::string &path) {
	// TODO
}

Entity *World::create_entity() {
	std::unique_ptr<Entity> e = std::make_unique<Entity>();
	auto ret = e.get();
	entity_list_.push_back(std::move(e));

	return ret;
}

void World::delete_all_entities() {
	entity_list_.clear();
}

void World::delete_entity(Entity *e) {
	for (auto it = entity_list_.begin(); it != entity_list_.end(); ++it) {
		if (it->get() == e) {
			for (auto child : e->transform()->get_children()) {
				this->delete_entity(child->entity());
			}
			entity_list_.erase(it);
			return;
		}
	}
}

void World::start(Game *game) {
	for (auto &&et : entity_list_) {
		for (auto &&comp : et->components_) {
			comp.second->start(game);
		}
	}
}

void World::handle_events(Game *game, SDL_Event const &e) {
	for (auto &&et : entity_list_) {
		for (auto &&comp : et->components_) {
			comp.second->handle_events(game, e);
		}
	}
}

void World::update(Game *game) {
	for (auto &&et : entity_list_) {
		for (auto &&comp : et->components_) {
			comp.second->update(game);
		}
	}
}

void World::draw(Game *game) {
	for (int i = 0; i < 3; ++i) {
		draw_layer(game, i);
	}
}

void World::draw_layer(Game *game, int layer) {
	for (auto &&et : entity_list_) {
		if (et->transform()->z == layer) {
			for (auto &&comp : et->components_) {
				comp.second->draw(game);
			}
		}
	}
}

} // namespace sl2dge
