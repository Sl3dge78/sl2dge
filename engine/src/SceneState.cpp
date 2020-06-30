#include "SceneState.h"

#include <memory>

void sl2dge::SceneState::start(Game *game) {
	auto map = world_.create_entity();

	//map->add_component<TileMap>(game->renderer(), "resources/levels/maptest.map");

	auto cam = world_.create_entity();
	cam->add_component<Camera>(320, 288);

	auto map_back = world_.create_system<TileMapSystem>(TileMapSystem::DrawParams::Back | TileMapSystem::DrawParams::Middle);
	map_back->camera_ = cam;

	/*
	player_ = std::make_unique<Player>(game);
	player_->teleport_to(Point(50, 50));

	scene_ = std::make_unique<Scene>(game, path_);

	game->set_current_map(scene_->map());

	main_camera_ = std::make_unique<Camera>(320, 288);
	main_camera_->start(game);
	//main_camera_->set_target(player_.get());

	game->set_main_camera(main_camera_.get());
	*/
}

void sl2dge::SceneState::handle_events(Game *game, const SDL_Event &e) {
	//player_->handle_events(game, e);

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
			case SDL_SCANCODE_E:
				// TODO IMPLEMENT
				on_interact(game);
				break;
		}
	}
}

void sl2dge::SceneState::input(Game *game) {
	//player_->input(game);
}

void sl2dge::SceneState::update(Game *game) {
	/*
	player_->update(game);
	main_camera_->update(game);

	// TODO : deactivate triggers if the player wals out of them
	/*
	auto chain = scene_->get_chain_at(player_->tiled_position().x, player_->tiled_position().y);
	if (chain != nullptr) {
		if (!chain->interactable()) {
			chain->activate(game);
			return;
		}
	}
	*/
}

void sl2dge::SceneState::draw(Game *game) {
	/*
	scene_->map()->draw(game, TileMap::DrawParams::Back | TileMap::DrawParams::Middle);
	//player_->draw(game);
	scene_->map()->draw(game, TileMap::DrawParams::Front);
	*/
}

void sl2dge::SceneState::on_interact(Game *game) {
	/*
	auto chain = scene_->get_chain_at(player_->tiled_position().x, player_->tiled_position().y);
	if (chain != nullptr && chain->in_place() && chain->interactable()) {
		chain->activate(game);
		return;
	}

	auto target_position = player_->tiled_position();
	switch (player_->facing_direction()) {
		case Direction::Up:
			target_position.y += -1;
			break;
		case Direction::Down:
			target_position.y += 1;
			break;
		case Direction::Left:
			target_position.x += -1;
			break;
		case Direction::Right:
			target_position.x += 1;
			break;
	}

	chain = scene_->get_chain_at(target_position.x, target_position.y);
	if (chain != nullptr && !chain->in_place() && chain->interactable()) {
		chain->activate(game);
		return;
	}
	return;
	*/
}
