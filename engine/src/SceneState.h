#pragma once

#include "Game.h"
#include "GameState.h"

namespace sl2dge {

	class SceneState : public GameState {
	public :
		SceneState(const std::string& path) : path_(path) {};
		~SceneState() = default;

		// Inherited via GameState
		virtual void start(Game* game) override;
		virtual void handle_events(Game* game, const SDL_Event& e) override;
		virtual void input(Game* game) override;
		virtual void update(Game* game) override;
		virtual void draw(Game* game) override;

		TileMap* map() { return map_.get(); }

	private :
		std::string path_;
		std::unique_ptr<Camera> main_camera_ = nullptr;
		std::unique_ptr<TileMap> map_ = nullptr;
		std::unique_ptr<Player> player_ = nullptr;
	};


}