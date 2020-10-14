#pragma once

#include <list>
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
	Entity *create_entity();
	Entity *create_entity(const float x, const float y);
	Entity *create_entity(const float x, const float y, Entity *parent);
	void delete_all_entities();
	void delete_entity(Entity *e);
	void delete_entity_imm(Entity *e);
	void delete_scheduled_entities();

	template <class T>
	std::unique_ptr<std::list<Entity *>> find_with_component();

	const std::list<std::unique_ptr<Entity>> *all_entities() const { return &entity_list_; }

	void start(Game *game);
	void handle_events(Game *game, SDL_Event const &e);
	void update(Game *game);
	void draw(Game *game);
	void draw_layer(Game *game, int layer);
	void cleanup(Game *game);

private:
	std::list<std::unique_ptr<Entity>> entity_list_;
	const int MAX_LAYERS = 4;
};

template <class T>
std::unique_ptr<std::list<Entity *>> World::find_with_component() {
	auto vec = std::make_unique<std::list<Entity *>>();

	for (auto &&e : entity_list_) {
		if (e->has_component<T>())
			vec->push_back(e.get());
	}

	return std::move(vec);
}

} // namespace sl2dge
