#pragma once

#include <memory>

#include "sl2dge.h"

using namespace sl2dge;
class MapEditor : public GameState {
public:
	MapEditor(TileMap *map);
	~MapEditor() = default;

	// Inherited via GameState
	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, const SDL_Event &e) override;
	virtual void input(Game *game) override;
	void paint(const Uint32 mouse_button, const SDL_Point pos);
	virtual void update(Game *game) override;
	virtual void draw(Game *game) override;

	void draw_atlas(Game *game);
	void draw_ui(Game *game);
	virtual void on_state_resume(Game *game) override;
	virtual void on_state_pause(Game *game) override;
	virtual void on_state_exit(Game *game) override{};

private:
	TileMap *map_;
	Atlas *atlas_ = nullptr;

	const SDL_Rect atlas_position = { 992, 0, 288, 720 };
	const int atlas_tile_w = 288 / 16;
	const int atlas_tile_h = 720 / 16;
	int atlas_x_offset = 0;
	int atlas_y_offset = 0;

	int current_atlas_tile = 0;
	int brush_size_ = 1;
};
