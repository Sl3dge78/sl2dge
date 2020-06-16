#pragma once

#include <string>
#include <memory>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "GameEvent.h"
#include "Point.h"
#include "HelperFunctions.h"
#include "Player.h"
#include "pugixml.hpp"
/*
namespace sl2dge {

	class Game;

	class GameEventManager {
	public:

		GameEventManager(Game* game, const pugi::xml_node& events_node);
		~GameEventManager();
		
		// Triggers event id
		bool trigger_event(const Guid id);

		// Triggers the next event in the chain
		bool next_event();

		void update(Player* player);

		// Call this when the player presses the interact button. This will trigger triggers if they can be triggered.
		void on_interact(Player* player);

		void save(pugi::xml_node& node);

		SDL_Texture* dialog_box() { return dialog_box_; }
		SDL_Texture* selection_texture() { return selection_texture_; }

	private:
		Game* game = nullptr;
		//std::vector<std::unique_ptr<GameEvent>> event_list;
		//std::vector<std::unique_ptr<Trigger>> triggers;

		GameEvent* current_event = nullptr;
		
		

		void activate_trigger(Trigger* trigger);
	};
}
*/