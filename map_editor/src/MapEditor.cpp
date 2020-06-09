#include "MapEditor.h"

#include "EventEditor.h"

Editor::Editor(const std::string map_path) {

	this->map_path = map_path;
}

Editor::~Editor() {
}

void Editor::start(Game* game) {
	map_camera = std::make_unique<Camera>(1000, 720, 1);
	game->set_main_camera(map_camera.get());

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(map_path.c_str());
	if (!result) {
		SDL_Log("Unable to read xml %s : %s", map_path.c_str(), result.description());
		throw std::exception("Unable to load XML");
	}

	SDL_Log("%s successfully loaded", map_path.c_str());
	auto map_node = doc.child("Map");

	map = std::make_unique<TileMap>(*game->renderer(), map_node);
	game->set_current_map(map.get());

	events = std::list<SDL_Point>();
	auto events_node = doc.child("Events");
	if (events_node) {
		for (auto chain : events_node.children()) {
			int x = chain.attribute("x_pos").as_int();
			int y = chain.attribute("y_pos").as_int();
			events.push_back(SDL_Point{ x,y });
		}
	}

	atlas_ = map->atlas();
}

void Editor::handle_events(Game* game, const SDL_Event& e) {

	if (e.type == SDL_MOUSEWHEEL) {
		if (e.wheel.y < 0) {

			auto new_zoom = map_camera->zoom() - 1;
			new_zoom = new_zoom <= 0 ? 1 : new_zoom;
			map_camera->set_zoom(new_zoom);

		} else if (e.wheel.y > 0) {

			auto new_zoom = map_camera->zoom() + 1;
			new_zoom = new_zoom > 10 ? 10 : new_zoom;
			map_camera->set_zoom(new_zoom);

		}
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
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
	}

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
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
		case SDL_SCANCODE_1:
			current_layer = 0;
			break;
		case SDL_SCANCODE_2:
			current_layer = 1;
			break;
		case SDL_SCANCODE_3:
			current_layer = 2;
			break;

		case SDL_SCANCODE_4:
			current_layer = 3;
			break;

		case SDL_SCANCODE_F5:
			SDL_Log("Saving ...");
			map->save(map_path);
			SDL_Log("Saved!");
			break;
		}

	}
}

void Editor::input(Game* game) {
	// cam movement
	const auto state = SDL_GetKeyboardState(NULL);
	Vector2 movement = { 0,0 };
	if (state[SDL_SCANCODE_W]) {
		movement.y += -1;
	}
	if (state[SDL_SCANCODE_S]) {
		movement.y += 1;
	}
	if (state[SDL_SCANCODE_D]) {
		movement.x += 1;
	}
	if (state[SDL_SCANCODE_A]) {
		movement.x += -1;
	}

	movement.normalize();
	movement *= 700 / map_camera->zoom() * float(game->delta_time()) / 1000.0f;
	map_camera->translate(movement);
	int x = 0;
	int y = 0;
	auto mouse_state = SDL_GetMouseState(&x, &y);
	if (x < atlas_position.x) {
		auto pos = map->pixel_to_map_transform(map_camera->screen_to_world_transform(Point(x, y)));
		if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (current_layer < 3) {
				
				map->set_tile(current_layer, pos, current_atlas_tile);
			} else {
				if (!has_event_at_position(pos.x, pos.y)) {
					add_event_at_position(pos.x, pos.y);
				} else {
					game->push_state(std::make_unique<EventEditor>(map_path, SDL_Point { pos.x, pos.y }));
				}
			}
		} else if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (current_layer < 3) {
				map->set_tile(current_layer, pos, -1);
			} else {
				if (has_event_at_position(pos.x, pos.y)) {
					delete_event_at_position(pos.x, pos.y);
				}
			}
		}
	}

}

void Editor::update(Game* game) {
	map_camera->update(game);
}

void Editor::draw(Game* game) {

	map->draw(game, TileMap::DrawParams::Back | TileMap::DrawParams::Middle | TileMap::DrawParams::Front);

	// Fond
	SDL_SetRenderDrawColor(game->renderer(), 75, 75, 75, 255);
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
				SDL_Rect src = *atlas_->get_tile_rect(i);
				SDL_Rect rect = { atlas_position.x + (x * 16), atlas_position.y + (y * 16), 16, 16 };
				SDL_RenderCopy(game->renderer(), atlas_->texture(), &src, &rect);
				if (i == current_atlas_tile)
					SDL_RenderDrawRect(game->renderer(), &rect);
			}

		}
	}

	//Current Layer
	SDL_Rect current_layer_rect = { 0,0,75,16 };
	SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
	SDL_RenderFillRect(game->renderer(), &current_layer_rect);
	FC_Draw(game->font(), game->renderer(), 0, 0, "Layer: %d", current_layer);

	if (current_layer == 3) { // Event layer
		for (auto& e : events) {
			SDL_Rect rect = map_camera->world_to_screen_transform(map->map_to_pixel_transform(Rect(e.x, e.y, 1, 1)));
			SDL_RenderDrawRect(game->renderer(), &rect);
		}
	}

}

void Editor::on_state_resume(Game* game) {

	start(game);
}

void Editor::on_state_pause(Game* game) {

}

bool Editor::has_event_at_position(const int x, const int y) {
	for (auto& e : events) { // Delete event at position
		if (e.x == x && e.y == y) {
			return true;
		}
	}
}

void Editor::delete_event_at_position(const int x, const int y) {
	for (auto it = events.begin(); it != events.end();++it) {
		if (it->x == x && it->y == y) {
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(map_path.c_str());
			if (!result) {
				SDL_Log("Unable to read xml %s : %s", map_path.c_str(), result.description());
				throw std::exception("Unable to load XML");
			}
			auto events_node = doc.child("Events");
			if (events_node) {
				
				for (auto chain : events_node.children()) {
					int x_ = chain.attribute("x_pos").as_int();
					int y_ = chain.attribute("y_pos").as_int();
					
					if (x_ == x && y_ == y) {
						events_node.remove_child(chain);
						break;
					}
				}
			}
			doc.save_file(map_path.c_str());
			events.erase(it);
			break;
		}
	}
}

void Editor::add_event_at_position(const int x, const int y) {

	events.push_back({ x, y });

}
