#include "EventEditor.h"

#include <iostream>
#include <fstream>


EventEditor::EventEditor(Scene* scene, EventChain* event_chain) {

	scene_ = scene;
	event_chain_ = event_chain;

	SDL_Point pos = { 0, 16 };
	interactable_ = std::make_unique<ToggleBox>("Inter", event_chain->interactable());
	interactable_->set_position(pos);
	pos.y += 16;
	is_in_place_ = std::make_unique<ToggleBox>("In Place", event_chain->in_place());
	is_in_place_->set_position(pos);
	pos.y += 16;

	activate_once_ = std::make_unique<ToggleBox>("Once", event_chain->once());
	activate_once_->set_position(pos);
	pos.y += 16;
}

void EventEditor::start(Game* game) {
	camera = std::make_unique<Camera>(1280, 720);
	game->set_main_camera(camera.get());
	SDL_StopTextInput();

	for (auto& event : *event_chain_->events()) {
		for (int i = 0; i < event->next_amt(); ++i) {
			if (event->next(i).isNil())
				continue;

			auto next = event_chain_->get_event(event->next(i));
			if(next != nullptr)
				next->has_prev = true;
		}
	}
}

void EventEditor::handle_events(Game* game, const SDL_Event& e) {

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		if (SDL_GetModState() & KMOD_CTRL && e.key.keysym.scancode == SDL_SCANCODE_N) {
			event_chain_->add_event(new Dialog());
		}
		if (SDL_GetModState() & KMOD_CTRL && e.key.keysym.scancode == SDL_SCANCODE_S) {
			save();
		}

		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_ESCAPE:
			game->pop_state();
			break;
		case SDL_SCANCODE_F5:
			save();
			break;

		}
	}

	if (e.type == SDL_MOUSEBUTTONDOWN) {
		auto mouse_pos = game->main_camera()->screen_to_world_transform(SDL_Point{ e.button.x, e.button.y });
		if (e.button.button == SDL_BUTTON_LEFT) {
			bool clicked_on_smth = false;

			for (auto& b : *event_chain_->events()) {

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

					if (!b->next(plug_out).isNil()) { // If something is already plugged where we clicked
						// remove it						
						event_chain_->get_event(b->next(plug_out))->has_prev = false;
						b->next(plug_out) = Guid();
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
				is_moving_ = false;
				plug_out = -1;
				is_plugging = false;

			} else if (plugging_out_box != nullptr && plugging_in_box != nullptr) { // We clicked on both in and out of nodes

				// OUT
				// Remove the previous plug if it exists
				if (!plugging_out_box->next(plug_out).isNil()) {
					event_chain_->get_event(plugging_out_box->next(plug_out))->has_prev = false;
				}
				// Plug it!
				plugging_out_box->set_next(plugging_in_box->id(), plug_out);

				// IN
				plugging_in_box->has_prev = true;

				// Cleanup
				plugging_in_box = nullptr;
				plugging_out_box = nullptr;
				plug_out = -1;
				is_plugging = false;
			}
		} else if (e.button.button == SDL_BUTTON_RIGHT) {
			for (auto it = event_chain_->events()->begin(); it != event_chain_->events()->end(); ++it) {

				// Clicking inside a box
				if (SDL_PointInRect(&mouse_pos, (*it)->rect())) {
					auto box = (*it).get();
					if (box->has_prev) {
						remove_all_connections_to(box->id());
					}

					for (int i = 0; i < box->next_amt(); i++) {
						auto a = box->next(i);
						if (!a.isNil()) {
							event_chain_->get_event(a)->has_prev = false;
						}
					}

					event_chain_->events()->erase(it);
					break;
				}

				// Clicking on in plug
				if ((*it)->has_in() && SDL_PointInRect(&mouse_pos, &(*it)->in_plug())) {
					remove_all_connections_to((*it)->id());
					break;
				}
			}
		} else if (e.button.button == SDL_BUTTON_MIDDLE) {
			for (auto& b : *event_chain_->events()) {
				// Clicking inside a box
				if (SDL_PointInRect(&mouse_pos, b->rect())) {
					event_chain_->set_next(b->id());
					SDL_Log("Entry point set %s", event_chain_->next().str().c_str());
				}
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

	for (auto& b : *event_chain_->events()) {
		b->handle_events(game, e);
	}

	interactable_->handle_events(game, e);
	is_in_place_->handle_events(game, e);
	activate_once_->handle_events(game, e);

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

	for (auto& b : *event_chain_->events()) {
		b->draw(game);
		for (int i = 0; i < b->next_amt(); ++i) {
			if (!b->next(i).isNil()) {
				int x2 = 0, y2 = 0;
				auto next = event_chain_->get_event(b->next(i));
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
		SDL_Point orig = plugging_out_box != nullptr ? camera->world_to_screen_transform(SDL_Point{ plugging_out_box->out_plug(plug_out).x, plugging_out_box->out_plug(plug_out).y }) : SDL_Point{ mouse_x, mouse_y };
		SDL_Point dst = plugging_in_box != nullptr ? camera->world_to_screen_transform(SDL_Point{ plugging_in_box->in_plug().x, plugging_in_box->in_plug().y }) : SDL_Point{ mouse_x, mouse_y };

		SDL_RenderDrawLine(game->renderer(), orig.x, orig.y, dst.x, dst.y);
	}


	// INFO 
	SDL_Rect info_rect = { 0,0,100,100 };
	SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 255);
	SDL_RenderFillRect(game->renderer(), &info_rect);
	FC_Draw(game->font(), game->renderer(), 0, 0, "%d, %d", event_chain_->position().x, event_chain_->position().y);

	interactable_->draw(game);
	is_in_place_->draw(game);
	activate_once_->draw(game);
}

void EventEditor::on_state_resume(Game* game) {
}

void EventEditor::on_state_pause(Game* game) {
}

void EventEditor::on_state_exit(Game* game) {
	save();
}

// Will search all boxes and remove the connection that connects to id
void EventEditor::remove_all_connections_to(Guid id) {
	
	event_chain_->get_event(id)->has_prev = false;

	int found = 0;
	for (auto& b : *event_chain_->events()) {
		for (int i = 0; i < b->next_amt(); ++i) {
			if (b->next(i) == id) {
				b->set_next(Guid(), i);
				found++;
			}
		}
	}
	if (found == 0)
		SDL_Log("Couldn't find any box that connects to %s", id.str().c_str());
	else 
		SDL_Log("Removed %d connections to %s", found, id.str().c_str());
}

void EventEditor::save() {
	event_chain_->set_interactable(interactable_->value());
	event_chain_->set_in_place(is_in_place_->value());
	event_chain_->set_once(activate_once_->value());

	scene_->save();
}
