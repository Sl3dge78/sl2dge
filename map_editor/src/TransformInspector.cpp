#include "TransformInspector.h"

TransformInspector::TransformInspector(Transform *transform) {
	transform_ = transform;
}

void TransformInspector::start(Game *game) {
	auto panel = world.create_entity(1000, 0);
	panel->add_component<UIPanel>(280, 720, SDL_Color{ 25, 25, 25, 255 });

	auto exit = world.create_entity(330 - 64, 0, panel);
	exit->add_component<UIText>("X", game->white_font());
	exit->add_component<UIButton>([this, game]() { this->quit(game); });

	auto title = world.create_entity(0, 0, panel);
	title->add_component<UIText>("Transform", game->white_font());

	auto x = world.create_entity(16, 16, panel);
	x->add_component<UIText>(std::to_string(transform_->position().x), game->white_font());

	auto y = world.create_entity(128, 16, panel);
	y->add_component<UIText>(std::to_string(transform_->position().y), game->white_font());
}

void TransformInspector::handle_events(Game *game, const SDL_Event &e) {
	world.handle_events(game, e);

	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			quit(game);
		}
	}
}

void TransformInspector::input(Game *game) {
}

void TransformInspector::update(Game *game) {
	world.update(game);
}

void TransformInspector::draw(Game *game) {
	world.draw(game);
}

void TransformInspector::quit(Game *game) {
	game->pop_state();
}
