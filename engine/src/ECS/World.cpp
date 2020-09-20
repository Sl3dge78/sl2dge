#include "World.h"

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
			auto comp = Component::create_component(type);
			comp->load(component_node);
			entity->add_component(comp);
			SDL_Log(">> Component of type %s added", type.c_str());
		}
		count++;
	}
	SDL_Log("%d entities loaded!", count);
}

void World::save(const std::string &path) {
	SDL_Log("Saving scene : %s", path.c_str());
	pugi::xml_document doc;
	SDL_Log("=====");
	SDL_Log("Saving entities...");
	for (auto &&et : entity_list_) {
		auto entity = doc.append_child("Entity");
		for (auto &&comp : et->components_) {
			auto component = entity.append_child("Component");
			auto type = comp.second->type_name();
			if (type == "")
				SDL_LogError(0, "Component type is empty!");

			component.append_attribute("type").set_value(type.c_str());
			comp.second->save(component);
			SDL_Log(">> Component of type %s saved", type.c_str());
		}
		SDL_Log("> Entity saved");
	}
	doc.save_file(path.c_str());
	SDL_Log("=====");
	SDL_Log("Saved scene : %s", path.c_str());
}

Entity *World::create_entity() {
	std::unique_ptr<Entity> e = std::make_unique<Entity>(this);
	auto ret = e.get();
	entity_list_.push_back(std::move(e));
	return ret;
}

Entity *World::create_entity(const float x, const float y) {
	auto e = create_entity();
	e->add_component<Transform>(x, y);
	return e;
}

Entity *World::create_entity(const float x, const float y, Entity *parent) {
	auto e = create_entity(x, y);
	parent->transform()->add_children(e->transform());
	return e;
}

void World::delete_all_entities() {
	entity_list_.clear();
}

void World::delete_entity(Entity *e) {
	e->to_delete = true;
	for (auto child : e->transform()->get_children()) {
		child->entity()->to_delete = true;
	}
}
void World::delete_entity_imm(Entity *e) {
	for (auto child : e->transform()->get_children()) {
		this->delete_entity_imm(child->entity());
	}
	entity_list_.remove_if([&](const std::unique_ptr<Entity> &e2) { return e == e2.get(); });
}
void World::delete_scheduled_entities() {
	entity_list_.remove_if([&](const std::unique_ptr<Entity> &e) { return e->to_delete; });
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

void World::cleanup(Game *game) {
	delete_scheduled_entities();
}

} // namespace sl2dge
