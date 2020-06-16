#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <sl2dge.h>

#include "Inventory.h"

using namespace sl2dge;

class InventoryState : public GameState {

public :

	InventoryState(Inventory* inventory);

	// Inherited via GameState
	virtual void start(Game* game) override;
	virtual void handle_events(Game* game, const SDL_Event& e) override;
	virtual void input(Game* game) override;
	virtual void update(Game* game) override;
	virtual void draw(Game* game) override;
	virtual void on_state_resume(Game* game) override {};
	virtual void on_state_pause(Game* game) override {};


private:

	SDL_Texture* background = nullptr;
	SDL_Texture* selection = nullptr;
	Point select_pos = { 0, 0 };
	SDL_Rect cursor_pos = { 0, 0, 32, 32 };

	// GRID
	const int width = 5;
	const int height = 8;
	const int offset_x = 16;
	const int offset_y = 30;

	// DESC
	const Rect text_box = { 92*2, 3*2, 65*2, 138 }; 
	const Rect name_box = { text_box.x + 8, text_box.y, text_box.w - 16, 64};
	const Rect desc_box = { text_box.x + 8, text_box.y + name_box.h, text_box.w, text_box.h - name_box.h - 32 };

	Inventory* inventory;
	Item* currently_selected_item = nullptr;
	SDL_Texture* item_name = nullptr;
	SDL_Texture* item_desc = nullptr;


	void UpdateCursorPosition(Game* game);

};

	