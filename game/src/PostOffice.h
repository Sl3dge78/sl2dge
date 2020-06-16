#pragma once

#include <memory>

#include <sl2dge.h>

#include "Player.h"

using namespace sl2dge;

class PostOffice : public GameState {

public:
	// Inherited via GameState
	virtual void start(Game* game) override;
	virtual void handle_events(Game* game, const SDL_Event& e) override;
	virtual void input(Game* game) override;
	virtual void update(Game* game) override;
	virtual void draw(Game* game) override;
	virtual void on_state_resume(Game* game) override;
	virtual void on_state_pause(Game* game) override;

private : 
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<GameEventManager> event_manager = nullptr;
	std::unique_ptr<Camera> main_camera = nullptr;
	std::unique_ptr<TileMap> map = nullptr;

	std::unique_ptr<Character> pnj1 = nullptr;
};