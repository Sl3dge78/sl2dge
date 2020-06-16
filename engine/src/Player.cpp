#include "player.h"

#include <memory>

#include "Sprites.h"

using namespace sl2dge;
/*
Player::Player(Game* game) {
	
	sprite_ = std::make_unique<AnimatedSprite>(*game->renderer(), "resources/images/char.png", 16, 16, 4, 3);
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

Player::~Player() {

}


void Player::start(Game* game) {
	
}

void Player::handle_events(Game* game, const SDL_Event& e) {
	
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_I:
			//game->push_state(std::make_unique<InventoryState>(inventory.get()));
			break;
		case SDL_SCANCODE_0 :
			break;
			//inventory->AddItem(new Item(0, "Nom", "Ceci est une description", std::make_unique<Sprite>(*game->renderer, "resources/images/items/letter.png", 16, 16)));
			//inventory->AddItem(new Item(2, "Nom2", "Ceci est une description2", std::make_unique<Sprite>(*game->renderer, "resources/images/items/letter.png", 16, 16)));
		}
	}
	


}

void Player::input(Game* game) {
	
	const auto state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_W]) {
		movement_.y += -1;
		facing_direction_ = Direction::Up;
	}
	if (state[SDL_SCANCODE_S]) {
		movement_.y += 1;
		facing_direction_ = Direction::Down;
	}
	if (state[SDL_SCANCODE_D]) {
		movement_.x += 1;
		facing_direction_ = Direction::Right;
	}
	if (state[SDL_SCANCODE_A]) {
		movement_.x += -1;
		facing_direction_ = Direction::Left;
	}
	movement_.normalize();
	movement_ *= speed_ * float(game->delta_time()) / 1000.0f;
	
}


void Player::update(Game* game) {

	
	std::string anim = "";
	if (movement_.length() <= 0)
		anim += "idle_";

	switch (facing_direction_) {
	case Direction::Up:		anim += "up";		break;
	case Direction::Down:	anim += "down";		break;
	case Direction::Left:	anim += "left";		break;
	case Direction::Right:	anim += "right";	break;
	}

	sprite_->animator()->play_animation(anim);

	sprite_->update(game->delta_time());

	if(movement_.length() > 0)
		move_and_slide(*game->current_map());
		
}

void Player::draw(Game* game) {
	
	sprite_->draw(*game->renderer(), *game->main_camera(), position_);
	
}

void Player::on_state_resume() {
}

void Player::on_state_pause() {
}
*/
