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

namespace sl2dge {

	class Game;

	class GameEventManager {
	public:

		GameEventManager(Game* game, pugi::xml_node& const events_node);
		~GameEventManager();

		// Adds an event to the list. Events can be triggered via in-game Triggers or directly using TriggerEvent.
		// See GameEvent subclasses constructors for more details.
		void add_game_event(GameEvent* e);

		// Adds a trigger to the list of possible triggers. Conditions will be checked during the update.
		// See Trigger class constructor for more details.
		void add_trigger(Trigger* trigger);

		GameEvent* find_game_event(const Guid id);
		
		// Triggers event id
		bool trigger_event(const Guid id);

		// Triggers the next event in the chain
		bool next_event();

		void update(Player* player);

		// Call this when the player presses the interact button. This will trigger triggers if they can be triggered.
		void on_interact(Player* player);

		SDL_Texture* dialog_box() { return dialog_box_; }
		SDL_Texture* selection_texture() { return selection_texture_; }

	private:
		Game* game = nullptr;
		std::vector<std::unique_ptr<GameEvent>> event_list;
		std::vector<std::unique_ptr<Trigger>> triggers;

		GameEvent* current_event = nullptr;
		
		SDL_Texture* dialog_box_ = nullptr;
		const std::string box_path = "resources/images/ui/dialogbox.png";

		SDL_Texture* selection_texture_ = nullptr;
		const std::string selection_icon_path = "resources/images/ui/selection.png";

		void activate_trigger(Trigger* trigger);
	};
}