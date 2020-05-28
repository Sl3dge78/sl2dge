#include "Island.h"
#include "PostOffice.h"

#include "QuestIDs.h"

void Island::start(Game* game) {

	event_manager = std::make_unique<GameEventManager>(game);

	map = std::make_unique<TileMap>(*game->renderer(), "levels/map2.map");
	game->set_current_map(map.get());

	player = std::make_unique<Player>(game);
	player->start(game);
	player->teleport_to(Point(35, 27), 16);

	main_camera = std::make_unique<Camera>(320, 288, 2);
	main_camera->start(game);
	main_camera->set_target(dynamic_cast<IMoveable*>(player.get()));
	game->set_main_camera(main_camera.get());


	// Teleport player to post office
	event_manager->add_trigger(new Trigger(Point(35, 26), 0, true));
	event_manager->add_game_event(new SceneChange(0, std::make_unique<PostOffice>(), -1));

	// Quest Q_STD_DELIVERY_CENTER
	event_manager->add_trigger(new Trigger(Point(40, 40), 100 ,true, false, false));
	event_manager->add_game_event(new QuestGTBranch(100, Q_STD_DELIVERY_CENTER, QS_STARTED, 101, -1));
	event_manager->add_game_event(new Dialog(101, { "Hey thank you for the delivery kid!" }, { 102 }));
	event_manager->add_game_event(new QuestStatus(102, Q_STD_DELIVERY_ANY, 0, 103));
	event_manager->add_game_event(new QuestStatus(103, Q_STD_DELIVERY_CENTER, 0));

	// Quest Q_STD_DELIVERY_WEST
	event_manager->add_trigger(new Trigger(Point(29,39), 200, true, false, false));
	event_manager->add_game_event(new QuestGTBranch(200, Q_STD_DELIVERY_WEST, QS_STARTED, 201, -1));
	event_manager->add_game_event(new Dialog(201, { "Hey thank you for the delivery kid!" }, { 202 }));
	event_manager->add_game_event(new QuestStatus(202, Q_STD_DELIVERY_ANY, 0, 203));
	event_manager->add_game_event(new QuestStatus(203, Q_STD_DELIVERY_WEST, 0));

	// Quest Q_STD_DELIVERY_EAST
	event_manager->add_trigger(new Trigger(Point(55, 35), 300, true, false, false));
	event_manager->add_game_event(new QuestGTBranch(300, Q_STD_DELIVERY_EAST, QS_STARTED, 301, -1));
	event_manager->add_game_event(new Dialog(301, { "Hey thank you for the delivery kid!" }, { 302 }));
	event_manager->add_game_event(new QuestStatus(302, Q_STD_DELIVERY_ANY, 0, 303));
	event_manager->add_game_event(new QuestStatus(303, Q_STD_DELIVERY_EAST, 0));

	// Quest Q_STD_DELIVERY_SOUTH
	event_manager->add_trigger(new Trigger(Point(40, 48), 400, true, false, false));
	event_manager->add_game_event(new QuestGTBranch(400, Q_STD_DELIVERY_SOUTH, QS_STARTED, 401, -1));
	event_manager->add_game_event(new Dialog(401, { "Hey thank you for the delivery kid!" }, { 402 }));
	event_manager->add_game_event(new QuestStatus(402, Q_STD_DELIVERY_ANY, 0, 403));
	event_manager->add_game_event(new QuestStatus(403, Q_STD_DELIVERY_SOUTH, 0));
}

void Island::input(Game* game) {

	player->input(game);
}

void Island::update(Game* game) {

	player->update(game);
	game->main_camera()->update(game);

	event_manager->update(player->rounded_tiled_position());
}

void Island::handle_events(Game* game, const SDL_Event& e) {
	player->handle_events(game, e);

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_E:
			event_manager->on_interact(player->rounded_tiled_position(), player->facing_direction());
			break;
		}
	}
}

void Island::draw(Game* game) {
	game->current_map()->draw(game, TileMap::DrawParams::Back | TileMap::DrawParams::Middle);
	player->draw(game);
	game->current_map()->draw(game, TileMap::DrawParams::Front);
}

void Island::on_state_resume() {
}

void Island::on_state_pause() {
}

Island::~Island() {
	event_manager.reset();
}
