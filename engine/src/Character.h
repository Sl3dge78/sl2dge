#pragma once
#include "SDL/SDL.h"

#include "Entity.h"
#include "Entity.h"
#include "Components.h"
#include "Sprites.h"
#include "Game.h"

namespace sl2dge {

	class Character : public Entity, public IMoveable {

	public:
		Character(Game* game, std::string path);
		~Character() = default;

		void start(Game* game) {};
		void handle_events(Game* game, const SDL_Event& e) {};
		void input(Game* game) {};
		void update(Game* game) { sprite_->update(game->delta_time()); };
		void draw(Game* game);
		void play_animation(const std::string anim) { sprite_->animator()->play_animation(anim); }

	private:
		std::unique_ptr<AnimatedSprite> sprite_;

		// Inherited via Entity
		virtual void on_state_resume() override {};
		virtual void on_state_pause() override {};
	};
}