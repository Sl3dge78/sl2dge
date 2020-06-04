#include "MainState.h"

#include "ItemList.h"
#include "Island.h"

#include "QuestIDs.h"

void MainState::start(Game* game) {

	Items::InitItems(game);
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_CENTER, "Standard delivery"));
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_SOUTH, "Standard delivery"));
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_WEST, "Standard delivery"));
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_EAST, "Standard delivery"));
	game->quest_manager()->add_game_quest(new Quest(Q_STD_DELIVERY_ANY, "Standard delivery"));
	
	
	game->push_state(std::make_unique<Scene>("resources/levels/maptest.map"));
}

void MainState::handle_events(Game* game, const SDL_Event& e) {
}

void MainState::input(Game* game) {
}


void MainState::update(Game* game) {
}

void MainState::draw(Game* game) {
}

void MainState::on_state_resume() {
}

void MainState::on_state_pause() {
}

MainState::~MainState() {

	Items::ClearItems();

}
