#pragma once

#include <sl2dge.h>

using namespace sl2dge;

class MainState : public GameState {
public:
	MainState();
	// Inherited via GameState
	virtual void start(Game *game) override;
	virtual void input(Game *game) override;
	virtual void update(Game *game) override;
	virtual void handle_events(Game *game, const SDL_Event &e) override;
	virtual void draw(Game *game) override;
	virtual void on_state_resume(Game *game) override;
	virtual void on_state_pause(Game *game) override;
	virtual ~MainState();

private:
	Entity *camera = nullptr;
	Entity *player = nullptr;
	std::unique_ptr<World> world_;
};
