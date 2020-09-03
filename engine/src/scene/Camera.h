#pragma once

#include <SDL/SDL.h>

#include "ECS/ECS.h"

namespace sl2dge {

class Game;

class Camera : public Component {
	COMPONENT_DFLT_BEHAVIOUR(Camera)
public:
	static Camera *main_camera;

	Camera(const int width, const int height, const float zoom = 1);
	Camera(){};

	SDL_Rect viewport_ = { 0, 0, 0, 0 }; // What the camera sees updated by the system
	float zoom_ = 1;
	int width_ = 100, height_ = 100;

	void load(const pugi::xml_node &node) override;
	void save(pugi::xml_node &node) override;

	void update(Game *game) override;

	SDL_Point world_to_screen_transform(const SDL_Point &point) const;
	SDL_Rect world_to_screen_transform(const SDL_Rect &rect) const;
	SDL_Point screen_to_world_transform(const SDL_Point &point) const;

	// Inherited via Component
	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, SDL_Event const &e) override;
	virtual void draw(Game *game) override;
};

} // namespace sl2dge
