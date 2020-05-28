#include "EventEditor.h"

void EventEditor::start(Game* game) {
	camera = std::make_unique<Camera>(1280, 720);
	game->set_main_camera (camera.get());
}

void EventEditor::handle_events(Game* game, const SDL_Event& e) {

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_N:
			boxes.push_back(DialogNodeBox());
			break;
		}
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			bool clicked_on_smth = false;
			for (auto& b : boxes) {
				auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
				// checking if clicking on in plug
				if (SDL_PointInRect(&mouse_pos, &b.in_plug())) {
					is_plugging = true;
					plugging_in_id = b.id();
					clicked_on_smth = true;
					break;
				}

				// Checking if clicking on out plug. there can be multiple, so we get the id if we clicked on one.
				int plug = b.is_point_in_plug(&mouse_pos);
				if (plug != -1) { // We clicked on an out plug
					is_plugging = true;
					plug_out = plug;
					plugging_out_id = b.id();

					if (b.next[plug_out] != -1) { // If something is already plugged where we clicked
						// remove it						
						get_box_from_id(b.next[plug_out])->has_prev = false;
						b.next[plug_out] = -1;
					}
					clicked_on_smth = true;
					break;
				}
			}

			// We clicked in the void, clear the current selection
			if (!clicked_on_smth) {
				plugging_in_id = -1;
				plugging_out_id = -1;
				plug_out = -1;
				is_plugging = false;
			} else if (plugging_out_id != -1 && plugging_in_id != -1) { // We clicked on both in and out of nodes
				
				// OUT
				auto b = get_box_from_id(plugging_out_id);
				// Remove the previous plug if it exists
				if (b->next[plug_out] != -1) {
					get_box_from_id(b->next[plug_out])->has_prev = false;
				}
				// Plug it!
				b->next[plug_out] = plugging_in_id;

				// IN
				get_box_from_id(plugging_in_id)->has_prev = true;

				// Cleanup
				plugging_in_id = -1;
				plugging_out_id = -1;
				plug_out = -1;
				is_plugging = false;
			}
		}
	}

	for (auto& b : boxes) {
		b.handle_events(game, e);
	}

}

void EventEditor::input(Game* game) {

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
	movement *= 700 * float(game->delta_time()) / 1000.0f;
	camera->translate(movement);

}

void EventEditor::update(Game* game) {
	camera->update(game);

}

void EventEditor::draw(Game* game) {

	SDL_SetRenderDrawColor(game->renderer(), 25, 25, 25, 255);
	SDL_RenderClear(game->renderer());

	for (auto& b : boxes) {
		b.draw(game);
		for (int i = 0; i < b.next.size(); ++i) {
			if (b.next[i] != -1) {
				int x2 = 0, y2 = 0;
				auto next = get_box_from_id(b.next[i]);
				if (next != nullptr) {
					x2 = next->in_plug().x;
					y2 = next->in_plug().y;
				}
				SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
				SDL_RenderDrawLine(game->renderer(), b.out_plug(i).x - camera->viewport().x, b.out_plug(i).y - camera->viewport().y, x2 - camera->viewport().x, y2 - camera->viewport().y);
			}
		}
		
	}

	if (is_plugging) {
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
		// TODO : Cleanup
		SDL_Point orig = plugging_out_id != -1 ? camera->world_to_screen_transform(SDL_Point{ get_box_from_id(plugging_out_id)->out_plug(plug_out).x, get_box_from_id(plugging_out_id)->out_plug(plug_out).y }) : SDL_Point{ mouse_x, mouse_y };
		SDL_Point dst = plugging_in_id != -1 ? camera->world_to_screen_transform(SDL_Point{ get_box_from_id(plugging_in_id)->in_plug().x, get_box_from_id(plugging_in_id)->in_plug().y }) : SDL_Point{ mouse_x, mouse_y };

		SDL_RenderDrawLine(game->renderer(), orig.x, orig.y, dst.x, dst.y);
	}
}

void EventEditor::on_state_resume() {
}

void EventEditor::on_state_pause() {
}
