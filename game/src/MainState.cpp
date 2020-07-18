#include "MainState.h"

using namespace sl2dge;

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
	camera = world_.create_entity(Vector2f(50, 50));
	camera->add_component<Camera>(320, 288);
	world_.create_system<CameraSystem>();
	world_.set_camera(camera);

	auto sprite = world_.create_entity();
	sprite->add_component<Sprite>(*game->renderer(), "resources/images/char.png", 16, 16);
	world_.create_system<SpriteSystem>();
	auto animator = sprite->add_component<SpriteAnimator>();
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

	auto map = world_.create_entity();
	map->add_component<TileMap>(*game->renderer(), "resources/levels/maptest.map");
	auto map_back = world_.create_system<TileMapSystem>(TileMapSystem::DrawParams::Back | TileMapSystem::DrawParams::Middle, 0);
	auto map_front = world_.create_system<TileMapSystem>(TileMapSystem::DrawParams::Front, 2);

	world_.update_systems_entities();
}

void MainState::handle_events(Game *game, const SDL_Event &e) {
}

void MainState::input(Game *game) {
	const auto state = SDL_GetKeyboardState(NULL);
	Vector2f movement;

	if (state[SDL_SCANCODE_W]) {
		movement.y += -1;
	}
	if (state[SDL_SCANCODE_S]) {
		movement.y += 1;
	}
	if (state[SDL_SCANCODE_D]) {
		movement.x += 1;
	}
	if (state[SDL_SCANCODE_A]) {
		movement.x += -1;
	}
	movement.normalize();
	movement *= 160 * float(game->delta_time()) / 1000.0f;

	camera->get_component<Transform>()->position += movement;
}

void MainState::update(Game *game) {
	world_.update(game->delta_time());
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
