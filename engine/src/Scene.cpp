#include "Scene.h"

#include <memory>

void sl2dge::Scene::start(Game* game) {
	player_ = std::make_unique<Player>(game);
	player_->teleport_to(Point(5, 5));

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path_.c_str());
	if (!result) {
		SDL_Log("Unable to read xml %s : %s", path_.c_str(), result.description());
		throw std::exception("Unable to load XML");
	}

	SDL_Log("%s successfully loaded", path_.c_str());

	auto events_node = doc.child("Events");
	event_manager_ = std::make_unique<GameEventManager>(game, events_node);

	auto map_node = doc.child("Map");
	map_ = std::make_unique<TileMap>(*game->renderer(), map_node);
	game->set_current_map(map_.get());

	main_camera_ = std::make_unique<Camera>(320, 288, 2);
	main_camera_->start(game);
	main_camera_->set_target(player_.get());

	game->set_main_camera(main_camera_.get());


}

void sl2dge::Scene::handle_events(Game* game, const SDL_Event& e) {
	player_->handle_events(game, e);

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_E:
			event_manager_->on_interact(player_.get());
			break;
		}
	}
}

void sl2dge::Scene::input(Game* game) {
	player_->input(game);
}

void sl2dge::Scene::update(Game* game) {
	player_->update(game);
	main_camera_->update(game);

	event_manager_->update(player_.get());
}

void sl2dge::Scene::draw(Game* game) {
	map_->draw(game, TileMap::DrawParams::Back | TileMap::DrawParams::Middle);
	player_->draw(game);
	map_->draw(game, TileMap::DrawParams::Front);
}

void sl2dge::Scene::on_state_resume(Game* game) {
}

void sl2dge::Scene::on_state_pause(Game* game) {
}
