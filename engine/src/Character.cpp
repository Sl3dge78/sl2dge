#include "Character.h"

sl2dge::Character::Character(Game* game, const std::string path) {
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

void sl2dge::Character::draw(Game* game) {
	sprite_->draw(*game->renderer(), *game->main_camera(), position_);
}
