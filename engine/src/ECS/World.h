#pragma once

#include <forward_list>
#include <memory>
#include <string>

#include <SDL/SDL.h>

#include "Component.h"
#include "Entity.h"

namespace sl2dge {

class Game;

class World {
public:
	World() = default;
	World(const std::string &path);
	~World(){};

	void load(const std::string &path);
	void save(const std::string &path);

	//Entities
	Entity *create_entity(const Vector2f &position);
	Entity *create_entity(const float x = 0, const float y = 0);
	void delete_all_entities();
	void delete_entity(Entity *e);

	void start(Game *game);
	void handle_events(Game *game, SDL_Event const &e);
	void update(Game *game);
	void draw(Game *game);

private:
	std::vector<std::unique_ptr<Entity>> entity_list_;
};

} // namespace sl2dge
