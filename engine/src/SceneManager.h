#pragma once

#include <map>
#include <memory>

#include "Game.h"
#include "TileMap.h"

namespace sl2dge {

	class SceneManager {
	public :

		static void Init();
		static std::unique_ptr<TileMap> LoadScene(Game* game, std::string name);
		static void Exit();
		static std::unique_ptr<std::map<std::string, std::string>> scenes_paths;
	private:
		
	
	};
	
}