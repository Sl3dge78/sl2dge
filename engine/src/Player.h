#pragma once

#include <memory>

#include <SDL/SDL.h>

#include "Components.h"
#include "Sprites.h"

using namespace sl2dge;

class Player : public Entity, public IMoveable {
public:
	Player(Game *game);
	~Player();

	void start(Game *game);
	void handle_events(Game *game, const SDL_Event &e);
	void input(Game *game);
	void update(Game *game);
	void draw(Game *game);

private:
	std::unique_ptr<AnimatedSprite> sprite_;
	float speed_ = 70;

	// Inherited via Entity
	virtual void on_state_resume() override;
	virtual void on_state_pause() override;
};
