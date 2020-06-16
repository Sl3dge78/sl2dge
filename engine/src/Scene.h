#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "EventChain.h"
#include "Game.h"
#include "xml/pugixml.hpp"

namespace sl2dge {

class Scene {
public:
	Scene(Game *game, const std::string &path);
	void open_xml_doc(pugi::xml_document *doc, const std::string &map_path);
	~Scene(){};

	void save();
	EventChain *get_chain_at(const int x, const int y) {
		for (auto &chain : event_chains_) {
			if (chain->position().x == x && chain->position().y == y)
				return chain.get();
		}
		return nullptr;
	}

	EventChain *create_chain_at(const int x, const int y) {
		auto chain = std::make_unique<EventChain>(x, y);
		auto ret = chain.get();
		event_chains_.push_back(std::move(chain));
		return ret;
	}

	void delete_chain_at(const int x, const int y) {
		for (auto it = event_chains_.begin(); it != event_chains_.end(); ++it) {
			if ((*it)->position().x == x && (*it)->position().y == y) {
				event_chains_.erase(it);
				break;
			}
		}
	}

	std::vector<std::unique_ptr<EventChain>> *get_all_chains() { return &event_chains_; }

	TileMap *map() { return map_.get(); }

private:
	std::string path_;
	std::vector<std::unique_ptr<EventChain>> event_chains_;
	std::unique_ptr<TileMap> map_ = nullptr;
};
} // namespace sl2dge
