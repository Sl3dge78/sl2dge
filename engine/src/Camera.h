#pragma once

#include <SDL\SDL.h>

#include "Entity.h"
#include "Point.h"
#include "Rect.h"

namespace sl2dge {

class Game;
class IMoveable;

class Camera : public Entity {
public:
	Camera(const int width, const int height, const float zoom = 1, const Vector2 &position = { 0, 0 });
	~Camera(){};

	Point world_to_screen_transform(const Point &point) const;
	SDL_Point world_to_screen_transform(const SDL_Point &point) const;
	Rect world_to_screen_transform(const Rect &rect) const;
	SDL_Rect world_to_screen_transform(const SDL_Rect &rect) const;
	Point screen_to_world_transform(const Point &point) const;
	SDL_Point screen_to_world_transform(const SDL_Point &point) const;
	Rect screen_to_world_transform(const Rect &rect) const;

	void set_target(IMoveable *obj);
	// Sets the center of the viewport to pos
	void set_position(const Vector2 &pos);
	void translate(const Vector2 &translation);

	Rect viewport() const { return viewport_; }
	Vector2 center() const { return position_; }
	/*
		const int zoom() const { return zoom_; }
		void set_zoom(int zoom) {
			this->zoom_ = zoom; 
			this->viewport_ = { 0, 0, width_ / zoom, height_ / zoom };
			//this->SetPosition(position);
		}
		*/
	const float zoom() const { return zoom_; }
	void set_zoom(const float zoom) {
		this->zoom_ = zoom;
		this->viewport_ = { 0, 0, int(float(width_) / zoom), int(float(height_) / zoom) };
		//this->SetPosition(position);
	}

	// Inherited via Entity
	virtual void start(Game *game) override{};
	virtual void handle_events(Game *game, const SDL_Event &e) override{};
	virtual void input(Game *game) override{};
	virtual void update(Game *game) override;
	virtual void draw(Game *game) override{};
	virtual void on_state_resume() override{};
	virtual void on_state_pause() override{};

private:
	Rect viewport_; // What the camera sees.
	Vector2 position_; // Position looking at.
	//int zoom_;
	float zoom_;
	IMoveable *target_;
	int width_, height_;
};
} // namespace sl2dge
