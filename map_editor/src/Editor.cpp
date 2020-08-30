#include "Editor.h"

namespace sl2dge {

Editor::Editor(const std::string &map_path) {
	map_path_ = map_path;
	scene_ = std::make_unique<World>(map_path_);

	editor_ = std::make_unique<World>();

	auto camera = editor_->create_entity();
	camera->add_component<Transform>(0.0f, 0.0f);
	map_camera_ = camera->add_component<Camera>(1280, 720);
	Camera::main_camera = map_camera_;
}

Editor::~Editor() {
}

void Editor::start(Game *game) {
	scene_->start(game);
	editor_->start(game);
}

void Editor::handle_events(Game *game, const SDL_Event &e) {
	if (e.type == SDL_MOUSEWHEEL) {
		if (e.wheel.y < 0) {
			float zoom = map_camera_->zoom_;
			if (zoom <= 1) {
				zoom /= 2.0;
			} else {
				--zoom;
			}
			map_camera_->zoom_ = zoom;

		} else if (e.wheel.y > 0) {
			float zoom = map_camera_->zoom_;
			if (zoom < 1) {
				zoom *= 2.0;
			} else {
				++zoom;
			}
			map_camera_->zoom_ = zoom;
		}
	}
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
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
		}
	}
}

void Editor::input(Game *game) {
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
	movement *= 700 / map_camera_->zoom_ * float(game->delta_time()) / 1000.0f;
	map_camera_->entity()->transform()->translate(movement);
}

void Editor::update(Game *game) {
	editor_->update(game);
}

void Editor::draw(Game *game) {
	for (int i = 0; i <= current_layer; ++i) {
		SDL_SetRenderDrawBlendMode(game->renderer(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 100);
		SDL_RenderFillRect(game->renderer(), NULL);
		scene_->draw_layer(game, i);
	}

	editor_->draw(game);
}
} // namespace sl2dge
