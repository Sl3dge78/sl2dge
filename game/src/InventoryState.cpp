#include "InventoryState.h"

InventoryState::InventoryState(Inventory* inventory) {

	this->inventory = inventory;

}

void InventoryState::start(Game* game) {

	auto tmp = IMG_Load("resources/images/ui/inventoryscreen.png");
	this->background = SDL_CreateTextureFromSurface(game->renderer(), tmp);
	SDL_FreeSurface(tmp);

	tmp = IMG_Load("resources/images/ui/selectionbox.png");
	this->selection = SDL_CreateTextureFromSurface(game->renderer(), tmp);
	SDL_FreeSurface(tmp);

	UpdateCursorPosition(game);
}

void InventoryState::handle_events(Game* game, const SDL_Event& e) {

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_ESCAPE :
		case SDL_SCANCODE_I :
			game->pop_state();
			break;

		case SDL_SCANCODE_A:
			select_pos.x--;
			if (select_pos.x < 0)
				select_pos.x = width-1;
			UpdateCursorPosition(game);
				break;
		case SDL_SCANCODE_D:
			select_pos.x++;
			if (select_pos.x >= width)
				select_pos.x = 0;
			UpdateCursorPosition(game);
			break;
		case SDL_SCANCODE_W:
			select_pos.y--;
			if (select_pos.y < 0)
				select_pos.y = height-1;
			UpdateCursorPosition(game);
			break;
		case SDL_SCANCODE_S:
			select_pos.y++;
			if (select_pos.y >= height)
				select_pos.y = 0;
			UpdateCursorPosition(game);
			break;
		}
	}

}

void InventoryState::input(Game* game) {
}

void InventoryState::update(Game* game) {

}

void InventoryState::draw(Game* game) {

	// Draw grid
	SDL_RenderCopy(game->renderer(), this->background, NULL, NULL);
	for (int i = 0; i < inventory->Length(); i++) {
		// Render each item
		if(inventory->GetItem(i) != nullptr)
			inventory->GetItem(i)->sprite()->draw_absolute(*game->renderer(), Rect(offset_x + ((i % width) * 32), offset_y + ((i / width) * 32), 32, 32));
	}
	SDL_RenderCopy(game->renderer(), this->selection, NULL, &cursor_pos);

	// Details
	if (currently_selected_item != nullptr) {
		FC_DrawBoxScale(game->font(), game->renderer(), name_box, { 2,2 }, currently_selected_item->name()->c_str());
		FC_DrawBox(game->font(), game->renderer(), desc_box, currently_selected_item->description()->c_str());
	}

}

void InventoryState::on_state_resume() {
}

void InventoryState::on_state_pause() {
}

void InventoryState::UpdateCursorPosition(Game* game) {

	SDL_DestroyTexture(item_name);
	item_name = nullptr;
	SDL_DestroyTexture(item_desc);
	item_desc = nullptr;
	currently_selected_item = nullptr;

	cursor_pos = { offset_x + 30 * select_pos.x, offset_y + 30 * select_pos.y, 32, 32 };
	auto item_no = select_pos.x + select_pos.y * width;
	if (inventory->Length() > item_no) {
		currently_selected_item = inventory->GetItem(item_no);
		if(currently_selected_item != nullptr)
			SDL_Log("Selected item %s", currently_selected_item->name()->c_str());

		/*
		// Create texts for name and desc
		auto surf = TTF_RenderText_Solid(game->font, currently_selected_item->GetName().c_str(), SDL_Color{ 0,0,0,0 });
		item_name = SDL_CreateTextureFromSurface(game->renderer, surf);
		SDL_FreeSurface(surf);

		auto surf = TTF_RenderText_Blended_Wrapped(game->font, currently_selected_item->GetName().c_str(), SDL_Color{ 0,0,0,0 }, text_box.w);
		item_desc = SDL_CreateTextureFromSurface(game->renderer, surf);
		SDL_FreeSurface(surf);
		*/
	}
}
