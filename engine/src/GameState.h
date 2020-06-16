#pragma once

#include <vector>
#include <memory>

namespace sl2dge {

	class Game;

	class GameState {
	public:
		virtual void start(Game* game) = 0;

		// Called only on the top state in the stack
		virtual void handle_events(Game* game, const SDL_Event& e) = 0;

		// Called only on the top state in the stack
		virtual void input(Game* game) = 0;

		// Called for each state in the stack
		virtual void update(Game* game) = 0;
		
		// Called for each state in the stack
		virtual void draw(Game* game) = 0;

		virtual void on_state_resume(Game* game) {};
		virtual void on_state_pause(Game* game) {};
		virtual void on_state_exit(Game* game) {};

		virtual ~GameState() = default;
		GameState() {};
	};
}