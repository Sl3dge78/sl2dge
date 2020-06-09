#include "SceneManager.h"

#include <filesystem>
#include <iostream>
#include <exception>

namespace sl2dge {

	// TODO : Make a doc reader that allows you to get Map data & event data in the engine

	std::unique_ptr < std::map<std::string, std::string>> SceneManager::scenes_paths = std::make_unique<std::map<std::string, std::string>>();

	void SceneManager::Init() {
		// TODO : Find all possible scenes in a specific folder and init the table
		std::string path = "resources/levels/";
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
			auto file = entry.path();
			if (file.extension() == ".map") {

				SceneManager::scenes_paths->insert(std::make_pair(file.filename().string(), entry.path().string()));
				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Scene %s found.", file.filename().string().c_str());
			}
		}
	}

	std::unique_ptr<TileMap> SceneManager::LoadScene(Game* game, std::string name) {
		//auto path = scenes_paths->find(name)->second;
		/*
		if (path == "") {
			if (game->map == nullptr) {
				SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Map %s doesn't exist", name.c_str());
				throw std::runtime_error("No map to revert to");
			} else {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Map %s doesn't exist, canceling map change", name.c_str());
				return std::move(game->map);
			}
		}
		*/
		//return std::make_unique<TileMap>(*game->renderer(), path);
		return nullptr;
	}

	void SceneManager::Exit() {
		scenes_paths.reset();
	}

}