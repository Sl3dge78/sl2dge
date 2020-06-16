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

	Character::Character(Game* game, const std::string path) {
		sprite_ = std::make_unique<AnimatedSprite>(*game->renderer(), path, 16, 16, 4, 3);
		sprite_->animator()->add_animation("idle_down", 5, { 1 });
		sprite_->animator()->add_animation("idle_up", 5, { 2 });
		sprite_->animator()->add_animation("idle_left", 5, { 0 });
		sprite_->animator()->add_animation("idle_right", 5, { 3 });
		sprite_->animator()->add_animation("right", 5, { 3,7,3,11 });
		sprite_->animator()->add_animation("left", 5, { 0,4,0,8 });
		sprite_->animator()->add_animation("up", 5, { 2,6,2,10 });
		sprite_->animator()->add_animation("down", 5, { 1,5,1,9 });

		position_ = Vector2(0, 0);
		movement_ = Vector2(0, 0);
		collider_ = Rect(2, 1, 11, 15);
	}

	void Character::draw(Game* game) {
		sprite_->draw(*game->renderer(), *game->main_camera(), position_);
	}
}