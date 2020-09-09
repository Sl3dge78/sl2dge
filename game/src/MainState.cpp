#include "MainState.h"

using namespace sl2dge;

MainState::MainState() {
	world_ = std::make_unique<World>("resources/test.map");
}

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
	/*
	
	world_.set_renderer(game->renderer());
	camera = world_.create_entity();
	camera->add_component<Camera>(320, 288);
	world_.create_system<CameraSystem>();
	world_.set_camera(camera);

	// Map
	auto map = world_.create_entity();
	auto tile_map = map->add_component<TileMap>(*game->renderer(), "resources/levels/maptest.map");
	auto map_back = world_.create_system<TileMapSystem>(TileMapSystem::DrawParams::Back | TileMapSystem::DrawParams::Middle, 0);
	auto map_front = world_.create_system<TileMapSystem>(TileMapSystem::DrawParams::Front, 2);

	// Player
	player = world_.create_entity(Vector2f(50 * 16, 50 * 16));
	player->add_component<Sprite>(*game->renderer(), "resources/images/char.png", 16, 16);
	world_.create_system<SpriteSystem>();

	auto animator = player->add_component<SpriteAnimator>();
	animator->add_animation("idle_down", 5, { 1 });
	animator->add_animation("idle_up", 5, { 2 });
	animator->add_animation("idle_left", 5, { 0 });
	animator->add_animation("idle_right", 5, { 3 });
	animator->add_animation("right", 5, { 3, 7, 3, 11 });
	animator->add_animation("left", 5, { 0, 4, 0, 8 });
	animator->add_animation("up", 5, { 2, 6, 2, 10 });
	animator->add_animation("down", 5, { 1, 5, 1, 9 });
	animator->play_animation("down");
	world_.create_system<SpriteAnimatorSystem>();

	player->add_component<Rigidbody>()->collider = { 2, 1, 11, 15 };
	world_.create_system<PhysicsSystem>(tile_map);

	player->transform()->add_children(camera->transform());

	// Events
	auto chain_e = world_.create_entity(Vector2f(0, 0));
	auto chain = chain_e->add_component<EventChain>();
	chain->interactable = true;
	world_.create_system<EventSystem>(camera);

	world_.update_systems_entities();
	world_.start();
	*/

	//world_.load_from_xml("resources/test.map");
	world_->start(game);
}

void MainState::handle_events(Game *game, const SDL_Event &e) {
	world_->handle_events(game, e);
}

void MainState::input(Game *game) {
	const auto state = SDL_GetKeyboardState(NULL);
	/*
	auto rb = player->get_component<Rigidbody>();

	if (state[SDL_SCANCODE_W]) {
		rb->speed.y += -1;
	}
	if (state[SDL_SCANCODE_S]) {
		rb->speed.y += 1;
	}
	if (state[SDL_SCANCODE_D]) {
		rb->speed.x += 1;
	}
	if (state[SDL_SCANCODE_A]) {
		rb->speed.x += -1;
	}
	rb->speed.normalize();
	rb->speed *= 70 * float(game->delta_time()) / 1000.0f;
	*/
}

void MainState::update(Game *game) {
	world_->update(game);
}

void MainState::draw(Game *game) {
	world_->draw(game);
}

void MainState::on_state_resume(Game *game) {
}

void MainState::on_state_pause(Game *game) {
}

MainState::~MainState() {
}
