#include "EventEditor.h"

void EventEditor::start(Game* game) {
	camera = std::make_unique<Camera>(1280, 720);
	game->set_main_camera(camera.get());
	SDL_StopTextInput();

	boxes = std::make_unique<std::vector<std::unique_ptr<EventNodeBox>>>();
	boxes->push_back(std::make_unique<TriggerBox>());
}

void EventEditor::handle_events(Game* game, const SDL_Event& e) {

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		if (SDL_GetModState() & KMOD_CTRL && e.key.keysym.scancode == SDL_SCANCODE_N) {
			boxes->push_back(std::make_unique<DialogNodeBox>());
		}
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
		if (e.button.button == SDL_BUTTON_LEFT) {
			bool clicked_on_smth = false;
			for (auto& b : *boxes) {

				// Clicking inside a box
				if (SDL_PointInRect(&mouse_pos, b->rect())) {
					selected_box = b.get(); 
					clicked_on_smth = true;
					if (SDL_PointInRect(&mouse_pos, &b->get_corner())) {
						is_resizing_ = true;
					} else {
						is_moving_ = true;
					}
				} 

				// Clicking on in plug
				if (b->has_in() && SDL_PointInRect(&mouse_pos, &b->in_plug())) {
					is_plugging = true;
					plugging_in_box = b.get();
					clicked_on_smth = true;
					break;
				}

				// Clicking on out plug. there can be multiple, so we get the id if we clicked on one.
				int plug = b->is_point_in_plug(&mouse_pos);
				if (plug != -1) { // We clicked on an out plug
					is_plugging = true;
					plug_out = plug;
					plugging_out_box = b.get();

					if (!b->next[plug_out].isNil()) { // If something is already plugged where we clicked
						// remove it						
						get_box_from_uuid(b->next[plug_out])->has_prev = false;
						b->next[plug_out] = Guid();
					}
					clicked_on_smth = true;
					break;
				}
			}

			// We clicked in the void, clear the current selection
			if (!clicked_on_smth) {

				plugging_in_box = nullptr;
				plugging_out_box = nullptr;
				selected_box = nullptr;
				is_resizing_ = false;
				is_moving_= false;
				plug_out = -1;
				is_plugging = false;

			} else if (plugging_out_box != nullptr && plugging_in_box != nullptr) { // We clicked on both in and out of nodes

				// OUT
				// Remove the previous plug if it exists
				if (!plugging_out_box->next[plug_out].isNil()) {
					get_box_from_uuid(plugging_out_box->next[plug_out])->has_prev = false;
				}
				// Plug it!
				plugging_out_box->next[plug_out] = plugging_in_box->guid();

				// IN
				plugging_in_box->has_prev = true;

				// Cleanup
				plugging_in_box = nullptr;
				plugging_out_box = nullptr;
				plug_out = -1;
				is_plugging = false;
			}
		}
	}

	if (e.type == SDL_MOUSEBUTTONUP) {
		is_resizing_ = false;
		is_moving_ = false;
		selected_box = nullptr;
	}

	if (e.type == SDL_MOUSEMOTION) {
		if (is_resizing_) {
			selected_box->resize(e.motion.xrel, e.motion.yrel);
		} else if (is_moving_) {
			selected_box->translate(e.motion.xrel, e.motion.yrel);
			
		}
	}

	for (auto& b : *boxes) {
		b->handle_events(game, e);
	}

}

void EventEditor::input(Game* game) {

	// cam movement
	if (!SDL_IsTextInputActive()) {
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

}

void EventEditor::update(Game* game) {
	camera->update(game);

}

void EventEditor::draw(Game* game) {

	SDL_SetRenderDrawColor(game->renderer(), 25, 25, 25, 255);
	SDL_RenderClear(game->renderer());

	for (auto& b : *boxes) {
		b->draw(game);
		for (int i = 0; i < b->next.size(); ++i) {
			if (!b->next[i].isNil()) {
				int x2 = 0, y2 = 0;
				auto next = get_box_from_uuid(b->next[i]);
				if (next != nullptr) {
					x2 = next->in_plug().x;
					y2 = next->in_plug().y;
				}
				SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
				SDL_RenderDrawLine(game->renderer(), b->out_plug(i).x - camera->viewport().x, b->out_plug(i).y - camera->viewport().y, x2 - camera->viewport().x, y2 - camera->viewport().y);
			}
		}

	}

	if (is_plugging) {
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
		// TODO : Cleanup
		SDL_Point orig = plugging_out_box != nullptr ? camera->world_to_screen_transform(SDL_Point{ plugging_out_box->out_plug(plug_out).x, plugging_out_box->out_plug(plug_out).y }) : SDL_Point{ mouse_x, mouse_y };
		SDL_Point dst = plugging_in_box != nullptr ? camera->world_to_screen_transform(SDL_Point{ plugging_in_box->in_plug().x, plugging_in_box->in_plug().y }) : SDL_Point{ mouse_x, mouse_y };

		SDL_RenderDrawLine(game->renderer(), orig.x, orig.y, dst.x, dst.y);
	}
}

void EventEditor::on_state_resume() {
}

void EventEditor::on_state_pause() {
}
