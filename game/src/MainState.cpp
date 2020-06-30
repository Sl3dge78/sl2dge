#include "MainState.h"

//#include "ItemList.h"
//#include "QuestIDs.h"
#include "scene/TileMap.h"

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

	auto cam = world_.create_entity();
	cam->add_component<Camera>(320, 288);

	auto map_back = world_.create_system<TileMapSystem>(TileMapSystem::DrawParams::Back | TileMapSystem::DrawParams::Middle);
	// TODO : Make it so that the camera is linked with the renderer, soi that when passing the renderer, the cam also gets passed.
	// You get acces to a way to chainge the main cam.
	map_back->camera_ = cam;

	world_.update_systems_entities();
}

void MainState::handle_events(Game *game, const SDL_Event &e) {}

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
