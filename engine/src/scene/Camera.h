#pragma once

#include <SDL/SDL.h>

#include "ECS/ECS.h"

namespace sl2dge {

class Game;

class Camera : public Component {
public:
	static Camera *main_camera;

	Camera(const int width, const int height, const float zoom = 1);
	Camera(const pugi::xml_node &node) { load(node); };

	SDL_Rect viewport_ = { 0, 0, 0, 0 }; // What the camera sees updated by the system
	float zoom_;
	int width_, height_;

	void load(const pugi::xml_node &node) override;
	void save(pugi::xml_node &node) override;

	void update(Game *game) override;

	SDL_Point world_to_screen_transform(const SDL_Point &point) const;
	SDL_Rect world_to_screen_transform(const SDL_Rect &rect) const;
	SDL_Point screen_to_world_transform(const SDL_Point &point) const;
};

} // namespace sl2dge
