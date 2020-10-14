#include "MapEditor.h"

#include "EventEditor.h"

MapEditor::MapEditor(TileMap *map) {
	this->map_ = map;
}

void MapEditor::start(Game *game) {
	atlas_ = map_->atlas();
}

void MapEditor::handle_events(Game *game, const SDL_Event &e) {
	if (e.type == SDL_MOUSEWHEEL) {
		if (e.wheel.y < 0) {
			float zoom = Camera::main_camera->zoom_;
			if (zoom <= 1) {
				zoom /= 2.0;
			} else {
				--zoom;
			}
			Camera::main_camera->zoom_ = zoom;

		} else if (e.wheel.y > 0) {
			float zoom = Camera::main_camera->zoom_;
			if (zoom < 1) {
				zoom *= 2.0;
			} else {
				++zoom;
			}
			Camera::main_camera->zoom_ = zoom;
		}
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			if (e.button.x > atlas_position.x) { // Clicked on Atlas

				auto x = e.button.x - atlas_position.x;
				auto y = e.button.y - atlas_position.y;
				x /= 16;
				y /= 16;
				x += atlas_x_offset;
				y += atlas_y_offset;
				if (x + y * atlas_->nb_tiles_atlas_width < atlas_->nb_tiles_atlas_width * atlas_->nb_tiles_atlas_height)
					current_atlas_tile = x + y * atlas_->nb_tiles_atlas_width;

				SDL_Log("Tile selected %d", current_atlas_tile);
			}
		} else if (e.button.button == SDL_BUTTON_MIDDLE) {
			if (current_layer <= 2) { // TILE PICKER
				current_atlas_tile = map_->get_tile(map_->pixel_to_map_transform(Camera::main_camera->screen_to_world_transform(Point(e.button.x, e.button.y))));
			}
		}
	}

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
				// Move atlas
			case SDL_SCANCODE_KP_4:
				atlas_x_offset--;
				if (atlas_x_offset < 0)
					atlas_x_offset = 0;
				break;
			case SDL_SCANCODE_KP_6:
				atlas_x_offset++;
				if (atlas_x_offset > atlas_->nb_tiles_atlas_width - atlas_tile_w)
					atlas_x_offset = atlas_->nb_tiles_atlas_width - atlas_tile_w;
				break;
			case SDL_SCANCODE_KP_8:
				atlas_y_offset--;
				if (atlas_y_offset < 0)
					atlas_y_offset = 0;
				break;
			case SDL_SCANCODE_KP_2:
				atlas_y_offset++;
				if (atlas_y_offset > atlas_->nb_tiles_atlas_height - atlas_tile_h)
					atlas_y_offset = atlas_->nb_tiles_atlas_height - atlas_tile_h;
				break;

			case SDL_SCANCODE_KP_PLUS:
				brush_size_++;
				break;
			case SDL_SCANCODE_KP_MINUS:
				brush_size_ = brush_size_ == 1 ? 1 : brush_size_ - 1;
				break;

			case SDL_SCANCODE_ESCAPE:
				game->pop_state();
				break;
		}
	}
}

void MapEditor::input(Game *game) {
	/* CAMERA MOVEMENT */

	const auto state = SDL_GetKeyboardState(NULL);
	Vector2f movement = { 0, 0 };
	if (state[SDL_SCANCODE_W])
		movement.y += -1;
	if (state[SDL_SCANCODE_S])
		movement.y += 1;
	if (state[SDL_SCANCODE_D])
		movement.x += 1;
	if (state[SDL_SCANCODE_A])
		movement.x += -1;
	movement.normalize();
	movement *= 700 / Camera::main_camera->zoom_ * float(game->delta_time()) / 1000.0f;
	Camera::main_camera->entity()->transform()->translate(movement);

	/* PAINT */
	int x = 0;
	int y = 0;
	auto mouse_state = SDL_GetMouseState(&x, &y);
	auto pos = map_->pixel_to_map_transform(Camera::main_camera->screen_to_world_transform(SDL_Point{ x, y }));

	if (x < atlas_position.x) {
		if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			paint(SDL_BUTTON_LEFT, pos);
			/*
			if (current_layer < 3) {
				paint(SDL_BUTTON_LEFT, pos);
			} else if (current_layer == Layer::Event) {
				auto chain = scene_->get_chain_at(pos.x, pos.y);
				if (chain != nullptr) {
					game->push_state(std::make_unique<EventEditor>(scene_.get(), chain)); // There is one, pass it
				} else {
					game->push_state(std::make_unique<EventEditor>(scene_.get(), scene_->create_chain_at(pos.x, pos.y))); // No chain exists, create one
				}
			} else if (current_layer == Layer::Collision) {
				map_->set_collision_at_tile(pos, true);
			}
			*/
		} else if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			paint(SDL_BUTTON_RIGHT, pos);
			/*
			if (current_layer < 3) {
				paint(SDL_BUTTON_RIGHT, pos);
			} else if (current_layer == Layer::Event) {
				if (scene_->get_chain_at(pos.x, pos.y) != nullptr) {
					scene_->delete_chain_at(pos.x, pos.y);
				}
			} else if (current_layer == Layer::Collision) {
				map_->set_collision_at_tile(pos, false);
			}
			*/
		}
	}
}

void MapEditor::paint(const Uint32 mouse_button, const SDL_Point pos) {
	for (unsigned int x = pos.x - brush_size_ + 1; x < unsigned int(pos.x + brush_size_); ++x) {
		if (x < 0 || x > map_->width())
			continue;
		for (unsigned int y = pos.y - brush_size_ + 1; y < unsigned int(pos.y + brush_size_); ++y) {
			if (y < 0 || y > map_->height())
				continue;
			if (mouse_button == SDL_BUTTON_LEFT)
				map_->set_tile(Point(x, y), current_atlas_tile);
			else if (mouse_button == SDL_BUTTON_RIGHT)
				map_->set_tile(Point(x, y), -1);
		}
	}
}

void MapEditor::update(Game *game) {
	//map_camera_->update(game);
}

void MapEditor::draw(Game *game) {
	draw_atlas(game);
	draw_ui(game);
}

void MapEditor::draw_atlas(Game *game) {
	/* ATLAS */
	// Fond
	SDL_SetRenderDrawColor(game->renderer(), 255, 0, 255, 255);
	SDL_RenderFillRect(game->renderer(), &atlas_position);

	// Bordure
	SDL_Rect border = SDL_Rect{ atlas_position.x - 2, 0, 2, 720 };
	SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
	SDL_RenderFillRect(game->renderer(), &border);

	// Atlas
	for (int y = 0; y < atlas_tile_h; y++) {
		for (int x = 0; x < atlas_tile_w; x++) {
			auto i = x + atlas_x_offset + ((y + atlas_y_offset) * atlas_->nb_tiles_atlas_width);
			if (i < atlas_->nb_tiles_atlas_width * atlas_->nb_tiles_atlas_height) {
				SDL_Rect src = *atlas_->get_tile(i);
				SDL_Rect rect = { atlas_position.x + (x * 16), atlas_position.y + (y * 16), 16, 16 };
				SDL_RenderCopy(game->renderer(), atlas_->texture(), &src, &rect);
				if (i == current_atlas_tile)
					SDL_RenderDrawRect(game->renderer(), &rect);
			}
		}
	}
}

void MapEditor::draw_ui(Game *game) {
	SDL_SetRenderDrawColor(game->renderer(), 25, 25, 25, 255);

	//Current Layer
	SDL_Rect rect = { 0, 0, 160, 16 };

	// BRUSH SIZE
	SDL_RenderFillRect(game->renderer(), &rect);
	FC_Draw(game->white_font(), game->renderer(), float(rect.x), float(rect.y), "brush size : %d", brush_size_);

	rect.y += 16;

	// KEY HELP
	SDL_Rect key_help_rect = { 0, int(game->window_height()) - 16, int(game->window_width()), 16 };
	SDL_RenderFillRect(game->renderer(), &key_help_rect);
	FC_Draw(game->white_font(), game->renderer(), float(key_help_rect.x), float(key_help_rect.y), "ESC : Close editor | W,Q,S,D, : Move Map | Middle Click : Pick Tile | NumPad4862 : Move Altas | NumPad+/- : Change Brush Size");
}

void MapEditor::on_state_resume(Game *game) {
}

void MapEditor::on_state_pause(Game *game) {
}
