#pragma once

#include "Game.h"
#include "GameState.h"
#include "Scene.h"

namespace sl2dge {

class SceneState : public GameState {
public:
	SceneState(const std::string &path) :
			path_(path){};
	~SceneState() = default;

	// Inherited via GameState
	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, const SDL_Event &e) override;
	virtual void input(Game *game) override;
	virtual void update(Game *game) override;
	virtual void draw(Game *game) override;
	void on_interact(Game *game);

private:
	std::string path_;
	std::unique_ptr<Camera> main_camera_ = nullptr;
	std::unique_ptr<Scene> scene_ = nullptr;
	//std::unique_ptr<Player> player_ = nullptr;
};

} // namespace sl2dge
