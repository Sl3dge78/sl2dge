#include "MainState.h"

//#include "ItemList.h"
//#include "QuestIDs.h"
#include "scene/Transform.h"
#include "scene/map/TileMap.h"

void MainState::start(Game *game) {
	/*
	Items::InitItems(game);
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_CENTER, "Standard delivery"));
	game->quest_manager()->add_game_quest(
			new Quest(Q_STD_DELIVERY_SOUTH, "Standard delivery"));
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_WEST, "Standard delivery"));
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_EAST, "Standard delivery"));
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_ANY, "Standard delivery"));

	game->push_state(std::make_unique<SceneState>("resources/levels/maptest.map"));
	*/
	//std::string path = ;

	//TODO : Automate this
	world_.set_renderer(game->renderer());

	auto map = world_.create_entity();
	map->add_component<TileMap>(*game->renderer(), "resources/levels/maptest.map");

	camera = world_.create_entity();
	camera->add_component<Camera>(320, 288);
	camera->add_component<Transform>(50.0f * 16.0f, 50.0f * 16.0f);

	world_.create_system<CameraSystem>();

	auto map_back = world_.create_system<TileMapSystem>(TileMapSystem::DrawParams::Back | TileMapSystem::DrawParams::Middle | TileMapSystem::DrawParams::Front);
	// TODO : Make it so that the camera is linked with the renderer, soi that when passing the renderer, the cam also gets passed.
	// You get acces to a way to chainge the main cam.
	map_back->camera_ = camera;

	world_.update_systems_entities();
}

void MainState::handle_events(Game *game, const SDL_Event &e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
			case SDL_SCANCODE_W:
				camera->get_component<Transform>()->position.y += -16;
				break;
			case SDL_SCANCODE_S:
				camera->get_component<Transform>()->position.y += 16;
				break;
			case SDL_SCANCODE_A:
				camera->get_component<Transform>()->position.x += -16;
				break;
			case SDL_SCANCODE_D:
				camera->get_component<Transform>()->position.x += 16;
				break;
		}
	}
}

void MainState::input(Game *game) {
}

void MainState::update(Game *game) {
	world_.update();
}

void MainState::draw(Game *game) {
	world_.draw();
}

void MainState::on_state_resume(Game *game) {
}

void MainState::on_state_pause(Game *game) {
}

MainState::~MainState() {
	//Items::ClearItems();
}
