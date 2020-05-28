#include "PostOffice.h"
#include "Island.h"

#include "QuestIDs.h"

void PostOffice::start(Game* game) {

	event_manager = std::make_unique<GameEventManager>(game);

	map = std::make_unique<TileMap>(*game->renderer(), "resources/levels/post-office.map");
	game->set_current_map (map.get());

	player = std::make_unique<Player>(game);
	player->start(game);
	player->teleport_to(Vector2(9 * 16 + 8, 17 * 16));

	main_camera = std::make_unique<Camera>(320, 288, 2);
	main_camera->start(game);
	main_camera->set_target(dynamic_cast<IMoveable*>(player.get()));
	game->set_main_camera (main_camera.get());

	pnj1 = std::make_unique<Character>(game, "resources/images/char2.png");
	pnj1->teleport_to(Point(15, 11), 16);
	pnj1->play_animation("idle_down");

	// Panneau gauche
	event_manager->add_trigger(new Trigger(Point(6, 14), 10, true, false, false));
	event_manager->add_game_event(new Dialog(10, { "Depot des Livraisons" }, { -1 }));

	// Panneau droite
	event_manager->add_trigger(new Trigger(Point(13, 14), 20, true, false, false));
	event_manager->add_game_event(new Dialog(20, { "Retrait des Livraisons" }, { -1 }));

	// Guichet droite
	event_manager->add_trigger(new Trigger(Point(15, 12), 100, true, false, false));
	event_manager->add_game_event(new QuestGTBranch(100, Q_STD_DELIVERY_ANY, QS_STARTED, 199, 101));
	event_manager->add_game_event(new Dialog(199, { "You already have a delivery in progress. Come back when it's done." }, { -1 })); // Player already has quest 
	event_manager->add_game_event(new Dialog(101, { "Hey friend, I just added a new delivery to your inventory." }, { 102 }));
	event_manager->add_game_event(new QuestStatus(102, Q_STD_DELIVERY_ANY, QS_STARTED, 103)); // We set the flag for standard delivery
	event_manager->add_game_event(new RandomBranch(103, { 110, 120, 130, 140 })); // Branch to random house

	event_manager->add_game_event(new QuestStatus(110, Q_STD_DELIVERY_CENTER, QS_STARTED, 111));
	event_manager->add_game_event(new Dialog(111, { "Deliver it to the house in the center of the town." }, { -1 }));

	event_manager->add_game_event(new QuestStatus(120, Q_STD_DELIVERY_WEST, QS_STARTED, 121));
	event_manager->add_game_event(new Dialog(121, { "Deliver it to the house west of town." }, { -1 }));

	event_manager->add_game_event(new QuestStatus(130, Q_STD_DELIVERY_EAST, QS_STARTED, 131));
	event_manager->add_game_event(new Dialog(131, { "Deliver it to the house east of town." }, { -1 }));

	event_manager->add_game_event(new QuestStatus(140, Q_STD_DELIVERY_SOUTH, QS_STARTED, 141));
	event_manager->add_game_event(new Dialog(141, { "Deliver it to the house south of town." }, { -1 }));


	// Guichet gauche
	event_manager->add_trigger(new Trigger(Point(4, 12), 200, true, false, false));
	event_manager->add_game_event(new Dialog(200, { "Guichet Gauche" }, { -1 }));

	// TP Out
	event_manager->add_trigger(new Trigger(Point(10, 18), 0, false, true, false));
	event_manager->add_trigger(new Trigger(Point(9, 18), 0, false, true, false));
	event_manager->add_game_event(new SceneChange(0, std::make_unique<Island>(), -1));
}

void PostOffice::handle_events(Game* game, const SDL_Event& e) {

	player->handle_events(game, e);

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_E:
			event_manager->on_interact(player->rounded_tiled_position(), player->facing_direction());
			break;
		}
	}

}

void PostOffice::input(Game* game) {
	player->input(game);

}

void PostOffice::update(Game* game) {
	pnj1->update(game);

	player->update(game);
	main_camera->update(game);
	event_manager->update(player->rounded_tiled_position());
}

void PostOffice::draw(Game* game) {
	map->draw(game, TileMap::DrawParams::Back | TileMap::DrawParams::Middle);
	player->draw(game);
	pnj1->draw(game);
	map->draw(game, TileMap::DrawParams::Front);
}

void PostOffice::on_state_resume() {
}

void PostOffice::on_state_pause() {
}
