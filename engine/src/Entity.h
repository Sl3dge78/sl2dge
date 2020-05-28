#pragma once

#include "SDL/SDL.h"


namespace sl2dge {

	class Game;

	class Entity {

	public:
		virtual void start(Game* game) = 0;
		virtual void handle_events(Game* game, const SDL_Event& e) = 0;
		virtual void input(Game* game) = 0;
		virtual void update(Game* game) = 0;
		virtual void draw(Game* game) = 0;

		virtual void on_state_resume() = 0;
		virtual void on_state_pause() = 0;
		static int next_id;
		virtual ~Entity() = default;

	protected:
		Entity() {	}

	};
}