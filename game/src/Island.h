#pragma once

#include <sl2dge.h>

#include "Player.h"

using namespace sl2dge;

class Island : public GameState {
public:
	// Inherited via GameState
	virtual void start(Game* game) override;
	virtual void input(Game* game) override;
	virtual void update(Game* game) override;
	virtual void handle_events(Game* game, const SDL_Event& e) override;
	virtual void draw(Game* game) override;
	virtual void on_state_resume(Game* game) override;
	virtual void on_state_pause(Game* game) override;
	virtual ~Island();

private:

	
	std::shared_ptr<Player> player = nullptr;
	std::unique_ptr<Camera> main_camera = nullptr;
	std::unique_ptr<TileMap> map = nullptr;
};