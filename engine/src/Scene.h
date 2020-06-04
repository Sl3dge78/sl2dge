#pragma once

#include "GameState.h"

namespace sl2dge {

	class Scene : public GameState {
	public :
		Scene(const std::string& path) : path_(path) {};
		~Scene() = default;

		// Inherited via GameState
		virtual void start(Game* game) override;
		virtual void handle_events(Game* game, const SDL_Event& e) override;
		virtual void input(Game* game) override;
		virtual void update(Game* game) override;
		virtual void draw(Game* game) override;
		virtual void on_state_resume() override;
		virtual void on_state_pause() override;

	private :
		std::string path_;
		std::unique_ptr<GameEventManager> event_manager_ = nullptr;
		std::unique_ptr<Camera> main_camera_ = nullptr;
		std::unique_ptr<TileMap> map_ = nullptr;
		std::unique_ptr<Player> player_ = nullptr;
	};


}