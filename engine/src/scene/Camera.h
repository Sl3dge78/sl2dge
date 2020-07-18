#pragma once

#include <SDL\SDL.h>

#include "ECS/Component.h"
#include "ECS/ISystem.h"

namespace sl2dge {

class Game;

class Camera : public Component {
public:
	SDL_Rect viewport_; // What the camera sees updated by the system
	float zoom_;
	int width_, height_;

	Camera(const int width, const int height, const float zoom = 1);
	~Camera(){};

	SDL_Point world_to_screen_transform(const SDL_Point &point) const;
	SDL_Rect world_to_screen_transform(const SDL_Rect &rect) const;
	SDL_Point screen_to_world_transform(const SDL_Point &point) const;
};

class CameraSystem : public ISystem, public UpdateSystem {
public:
	CameraSystem() {
		this->add_component_filter<Camera>();
	}
	virtual void update(Game *game) override;
};

} // namespace sl2dge
