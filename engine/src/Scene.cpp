#include "Scene.h"

sl2dge::Scene::Scene(Game *game, const std::string &path) {
	this->path_ = path;

	pugi::xml_document doc;
	open_xml_doc(&doc, path_);

	auto map_node = doc.child("Map");
	//map_ = std::make_unique<TileMap>(*game->renderer(), map_node);

	auto events_node = doc.child("Events");
	for (auto chain : events_node.children("Event_Chain")) {
		event_chains_.push_back(std::make_unique<EventChain>(chain));
	}
}

void sl2dge::Scene::open_xml_doc(pugi::xml_document *doc, const std::string &map_path) {
	pugi::xml_parse_result result = doc->load_file(map_path.c_str());
	if (!result) {
		SDL_Log("Unable to read xml %s : %s", map_path.c_str(), result.description());
		if (result.status == pugi::xml_parse_status::status_bad_attribute) {
			std::ifstream file;
			file.open(map_path);
			file.seekg(result.offset);

			std::string s;
			s.resize(20);
			file.read(&s[0], 20);
			SDL_Log("Error at [...%s]", s.c_str());
		}
		doc = new pugi::xml_document();
	}

	SDL_Log("%s successfully loaded", map_path.c_str());
}

void sl2dge::Scene::save() {
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
			chain->get_xml_data(chain_node); // Overwrite
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
			chain->get_xml_data(node);
		}
	}

	doc.save_file(path_.c_str());
	SDL_Log("Saved %s!", path_.c_str());
}
