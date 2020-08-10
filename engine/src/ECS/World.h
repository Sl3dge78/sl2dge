#pragma once

#include <forward_list>
#include <memory>
#include <string>

#include <SDL/SDL.h>

#include "Component.h"
#include "Entity.h"
#include "ISystem.h"
#include "addons/SDL_FontCache.h"
#include "math/Vector.h"
#include "scene/Camera.h"

namespace sl2dge {

class Game;

class World {
public:
	World() = default;
	~World(){};

	//Entities
	Entity *create_entity(const Vector2f &position);
	Entity *create_entity(const float x = 0, const float y = 0);
	void delete_all_entities();
	void delete_entity(Entity *e);

	template <class T, class... Args>
	T *create_system(Args &&... args);

	void delete_all_systems();

	void start(Game *game);
	void handle_events(Game *game, SDL_Event const &e);
	void update(Game *game);
	void draw(Game *game);

	void update_systems_entities();

protected:
	//Systems
	std::forward_list<std::unique_ptr<ISystem>> systems_;
	std::forward_list<InitSystem *> init_systems_;
	std::forward_list<InputSystem *> input_systems_;
	std::forward_list<UpdateSystem *> update_systems_;
	std::forward_list<DrawSystem *> draw_systems_;
	ISystem *add_system(ISystem *sys);

private:
	std::vector<std::unique_ptr<Entity>> entity_list_;

	bool is_systems_entities_list_dirty_ = false;
};

} // namespace sl2dge
