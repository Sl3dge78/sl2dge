#include "TransformEditor.h"

TransformEditor::TransformEditor(Transform *transform) {
	transform_ = transform;
}

void TransformEditor::start(Game *game) {
	auto panel = world.create_entity(1000, 0);
	panel->add_component<UIPanel>(280, 720, SDL_Color{ 25, 25, 25, 255 });

	auto exit = world.create_entity(330 - 64, 0, panel);
	exit->add_component<UIText>("X", game->white_font());
	exit->add_component<UIButton>([this, game]() { this->quit(game); });

	auto title = world.create_entity(0, 0, panel);
	title->add_component<UIText>("Transform", game->white_font());

	world.create_entity(16, 16, panel)->add_component<UIText>("X:", game->white_font());
	auto x = world.create_entity(48, 16, panel);
	x->add_component<UIFloatField>(&transform_->local_position()->x, game->white_font());

	world.create_entity(16, 32, panel)->add_component<UIText>("Y:", game->white_font());
	auto y = world.create_entity(48, 32, panel);
	y->add_component<UIFloatField>(&transform_->local_position()->y, game->white_font());

	world.create_entity(16, 48, panel)->add_component<UIText>("Z:", game->white_font());
	auto z = world.create_entity(48, 48, panel);
	z->add_component<UIIntField>(&transform_->z, game->white_font());
}

void TransformEditor::handle_events(Game *game, const SDL_Event &e) {
	world.handle_events(game, e);

	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			quit(game);
		}
	}
}

void TransformEditor::input(Game *game) {
}

void TransformEditor::update(Game *game) {
	world.update(game);
}

void TransformEditor::draw(Game *game) {
	world.draw(game);
}

void TransformEditor::quit(Game *game) {
	game->pop_state();
}
