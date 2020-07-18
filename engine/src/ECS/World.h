#pragma once
#include <forward_list>

#include "../SDL_FontCache.h"
#include "SDL/SDL.h"

#include "Component.h"
#include "Entity.h"
#include "ISystem.h"

#include "math/Vector.h"
#include "scene/Camera.h"

namespace sl2dge {

class World {
public:
	World() = default;
	~World();

	//Entities
	Entity *create_entity();
	Entity *create_entity(const Vector2f &position);
	void delete_all_entities();
	void delete_entity(Entity *e);

	//Systems
	template <class T, class... Args>
	T *create_system(Args &&... args);
	void delete_all_systems();

	void start();
	void handle_events(SDL_Event const &e);
	void update(const int delta_time);
	void draw();

	void update_systems_entities();

	void set_renderer(SDL_Renderer *renderer) { renderer_ = renderer; };
	void set_main_font(FC_Font *font) { main_font_ = font; };
	void set_camera(Entity *camera) {
		camera_ = camera;
		for (auto s : draw_systems_) {
			s->camera_ = camera_->get_component<Camera>();
		}
	};

private:
	SDL_Renderer *renderer_;
	FC_Font *main_font_;
	Entity *camera_;

	std::vector<Entity *> entity_list_; // TODO : Switch to unique ptr

	//Systems
	ISystem *add_system(ISystem *sys);
	std::forward_list<ISystem *> systems_;
	std::forward_list<InitSystem *> init_systems_;
	std::forward_list<InputSystem *> input_systems_;
	std::forward_list<UpdateSystem *> update_systems_;
	std::forward_list<DrawSystem *> draw_systems_;

	bool is_systems_entities_list_dirty_ = false;
};

//SYSTEMS
template <class T, class... Args>
T *World::create_system(Args &&... args) {
	return static_cast<T *>(add_system(new T(std::forward<Args>(args)...)));
}

} // namespace sl2dge
