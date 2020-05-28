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


namespace sl2dge {

	class Game;

	class GameEventManager {
	public:

		GameEventManager(Game* game);
		~GameEventManager();

		// Adds an event to the list. Events can be triggered via in-game Triggers or directly using TriggerEvent.
		// See GameEvent subclasses constructors for more details.
		void add_game_event(GameEvent* e);

		// Adds a trigger to the list of possible triggers. Conditions will be checked during the update.
		// See Trigger class constructor for more details.
		void add_trigger(Trigger* trigger);

		GameEvent* find_game_event(const int id);
		
		// Triggers event id
		bool trigger_event(const int id);

		// Triggers the next event in the chain
		bool next_event();

		void update(const Point& player_position);

		// Call this when the player presses the interact button. This will trigger triggers if they can be triggered.
		void on_interact(const Point& player_position, Direction direction);

		SDL_Texture* dialog_box() { return dialog_box_; }
		SDL_Texture* selection_texture() { return selection_texture_; }

	private:
		Game* game;
		std::vector<std::unique_ptr<GameEvent>> event_list;
		std::vector<std::unique_ptr<Trigger>> triggers;

		GameEvent* current_event = nullptr;
		
		SDL_Texture* dialog_box_ = nullptr;
		const std::string box_path = "images/ui/dialogbox.png";

		SDL_Texture* selection_texture_ = nullptr;
		const std::string selection_icon_path = "images/ui/selection.png";

		void activate_trigger(Trigger* trigger);
	};
}