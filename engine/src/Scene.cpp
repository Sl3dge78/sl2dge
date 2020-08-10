#include "Scene.h"

namespace sl2dge {

Scene::Scene(const std::string &path) {
	this->path_ = path;
	load_from_xml(path);
}

void Scene::load_from_xml(const std::string &path) {
	SDL_Log("Loading scene : %s", path.c_str());
	pugi::xml_document doc;
	open_xml_doc(&doc, path_);
	SDL_Log("=====");
	SDL_Log("Loading entities...");
	for (auto entity_node : doc.children("Entity")) {
		auto entity = this->create_entity(entity_node.attribute("x").as_int() * 16.0f, entity_node.attribute("y").as_int() * 16.0f);
		SDL_Log("> Entity added");

		for (auto component_node : entity_node.children("Component")) {
			// Add component
			std::string type = component_node.attribute("type").as_string();
			auto comp = ECS_DB::create_component(type, entity, component_node);
			if (type == "Camera") {
				Camera::main_camera = dynamic_cast<Camera *>(comp);
			}
			SDL_Log(">> Component of type %s added", type.c_str());
		}
	}
	SDL_Log("All entities loaded!");
	SDL_Log("=====");
	SDL_Log("Loading systems...");
	for (auto system_node : doc.children("System")) {
		std::string type = system_node.attribute("type").as_string();
		add_system(ECS_DB::create_system(type, system_node));
		SDL_Log("> System of type %s loaded", type.c_str());
	}
	SDL_Log("All systems loaded!");
	update_systems_entities();
}

void Scene::save() {
	/*
	pugi::xml_document doc;
	open_xml_doc(&doc, path_);

	auto map_node = doc.child("Map");
	//map_->save(map_node);

	auto events_node = doc.child("Events");

	for (auto &chain_node : events_node.children("Event_Chain")) {
		auto chain = get_chain_at(chain_node.attribute("x_pos").as_int(), chain_node.attribute("y_pos").as_int());
		if (chain == nullptr) {
			events_node.remove_child(chain_node); // Remove excess nodes
		} else {
			chain->save(chain_node); // Overwrite
		}
	}

	// Save new ones
	for (auto &chain : event_chains_) {
		bool write = true;
		for (auto &node : events_node.children("Event_Chain")) {
			Point pos = { node.attribute("x_pos").as_int(), node.attribute("y_pos").as_int() };
			if (pos == chain->position()) {
				write = false;
				break;
			}
		}

		if (write) {
			auto node = events_node.append_child("Event_Chain");
			chain->save(node);
		}
	}

	doc.save_file(path_.c_str());
	SDL_Log("Saved %s!", path_.c_str());
	*/
}

} // namespace sl2dge
