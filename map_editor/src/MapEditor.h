#pragma once

#include "GameState.h"

using namespace sl2dge;

class Editor : public GameState {

public:

	Editor(const std::string map_path);
	~Editor();

	// Inherited via GameState
	virtual void start(Game* game) override;
	virtual void handle_events(Game* game, const SDL_Event& e) override;
	virtual void input(Game* game) override;
	virtual void update(Game* game) override;
	virtual void draw(Game* game) override;
	virtual void on_state_resume() override;
	virtual void on_state_pause() override;

private:
	std::unique_ptr<TileMap> map = nullptr;
	std::unique_ptr<Camera> map_camera = nullptr;
	std::string map_path;

	const SDL_Rect atlas_position = { 992,0,288,720 };
	const int atlas_tile_w = 288 / 16;
	const int atlas_tile_h = 720 / 16;
	int atlas_x_offset = 0;
	int atlas_y_offset = 0;

	int current_atlas_tile = 0;

	int current_layer = 0;

	Atlas* atlas_ = nullptr;
};
