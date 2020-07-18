#include "DialogState.h"

#include <iostream>
#include <string>

#include <SDL/SDL_image.h>

#include "addons/SDL_FontCache.h"

namespace sl2dge {

void DialogState::start(Game *game) {
	auto s = IMG_Load(box_path.c_str());
	dialog_box_ = SDL_CreateTextureFromSurface(game->renderer(), s);
	SDL_FreeSurface(s);

	s = IMG_Load(selection_icon_path.c_str());
	selection_texture_ = SDL_CreateTextureFromSurface(game->renderer(), s);
	SDL_FreeSurface(s);

	current_choice = 0;

	if (is_choice) {
		// Define rendering area
		int max_w = 64;
		int max_h = 8;

		for (int i = 0; i < current_choice_dialog->next_amt(); ++i) {
			int w = FC_GetWidth(game->font(), current_choice_dialog->choice_text(i)->c_str());
			int h = FC_GetHeight(game->font(), current_choice_dialog->choice_text(i)->c_str());
			w += 32;
			max_h += h + 8;
			if (w > max_w)
				max_w = w;
		}
		if (max_h < 64)
			max_h = 64;

		choice_rect = Rect(choice_anchor.x - max_w, choice_anchor.y - max_h, max_w, max_h);
	}
}

void DialogState::handle_events(Game *game, const SDL_Event &e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
			case SDL_SCANCODE_S:
				if (is_choice) {
					current_choice++;
					if (current_choice > int(current_choice_dialog->next_amt()) - 2)
						current_choice = int(current_choice_dialog->next_amt()) - 2;

					if (current_choice < 0)
						current_choice = 0;
					current_choice_dialog->set_choice(current_choice);
				}
				break;
			case SDL_SCANCODE_W:
				if (is_choice) {
					current_choice--;
					if (current_choice < 0)
						current_choice = 0;
					current_choice_dialog->set_choice(current_choice);
				}
				break;
			case SDL_SCANCODE_E:
				//event_chain->next_event(game);
				break;
		}
	}
}

void DialogState::input(Game *game) {
}

void DialogState::update(Game *game) {
}

void DialogState::draw(Game *game) {
	draw_dialog_box(*game->renderer(), *dialog_box_, 2, single_rect);
	if (is_choice) {
		draw_dialog_box(*game->renderer(), *dialog_box_, 2, choice_rect);
		//RenderCopy(*game->renderer(), *selection_texture_, NULL, &Rect(choice_rect.x - 20, choice_rect.y + 6 + current_choice * 32, 32, 32));
	}

	// Bottom
	FC_DrawBox(game->font(), game->renderer(), single_rect + Rect(8, 8, -8, -8), current_dialog->text.c_str());

	// Choices
	if (is_choice) {
		for (int i = 0; i < current_choice_dialog->next_amt(); ++i) {
			FC_DrawAlign(game->font(), game->renderer(), float(choice_anchor.x - 8), float(8 + choice_rect.y + ((16 + 8) * (i))), FC_ALIGN_RIGHT, current_choice_dialog->choice_text(i)->c_str());
		}
	}
}

void DialogState::draw_dialog_box(SDL_Renderer &renderer, SDL_Texture &dialog_box, const int zoom, const Rect &rect) {
	auto tile_size = 16 * zoom;

	if (rect.w < tile_size * 2 || rect.h < tile_size * 2) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Dialog box of size <32px not implemented");
		return;
	}
	// TODO
	//Upper Left Corner
	//RenderCopy(renderer, dialog_box, &Rect(0, 0, 16, 16),&Rect(rect.x, rect.y, tile_size, tile_size));
	//Upper Line
	//RenderCopy(renderer, dialog_box, &Rect(16, 0, 16, 16),&Rect(rect.x + tile_size, rect.y, rect.w - 2 * tile_size, tile_size));
	//Upper Right Corner
	//RenderCopy(renderer, dialog_box, &Rect(32, 0, 16, 16),&Rect(rect.x + rect.w - tile_size, rect.y, tile_size, tile_size));

	if (rect.h > 2 * tile_size) {
		// Middle Left
		//RenderCopy(renderer, dialog_box, &Rect(0, 16, 16, 16),&Rect(rect.x, rect.y + tile_size, tile_size, rect.h - tile_size * 2));
		// Middle Center
		//RenderCopy(renderer, dialog_box, &Rect(16, 16, 16, 16),&Rect(rect.x + tile_size, rect.y + tile_size, rect.w - tile_size * 2, rect.h - tile_size * 2));
		// Middle Right
		//RenderCopy(renderer, dialog_box, &Rect(32, 16, 16, 16),&Rect(rect.x + rect.w - tile_size, rect.y + tile_size, tile_size, rect.h - tile_size * 2));
	}

	//Lower Left Corner
	//RenderCopy(renderer, dialog_box, &Rect(0, 32, 16, 16), &Rect(rect.x, rect.y + rect.h - tile_size, tile_size, tile_size));
	//Lower Line
	//RenderCopy(renderer, dialog_box, &Rect(16, 32, 16, 16),&Rect(rect.x + tile_size, rect.y + rect.h - tile_size, rect.w - 2 * tile_size, tile_size));
	//Lower Right Corner
	//RenderCopy(renderer, dialog_box, &Rect(32, 32, 16, 16),&Rect(rect.x + rect.w - tile_size, rect.y + rect.h - tile_size, tile_size, tile_size));
}

void DialogState::on_state_resume(Game *game) {
}

void DialogState::on_state_pause(Game *game) {
}

} // namespace sl2dge
