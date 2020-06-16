#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL/SDL.h>

#include "Camera.h"
#include "HelperFunctions.h"

namespace sl2dge {

class Sprite {
public:
	Sprite(SDL_Renderer &renderer, const std::string &path, int width, int height);
	~Sprite();

	void draw(SDL_Renderer &renderer, const Camera &cam, const Point &position);
	void draw(SDL_Renderer &renderer, const Camera &cam, const Rect &position);
	void draw_absolute(SDL_Renderer &renderer, const Rect &position);

protected:
	Rect src;
	SDL_Texture *texture;
};

class SpriteAnimation {
public:
	SpriteAnimation(std::string name, int frame_rate, std::initializer_list<int> list) :
			name(name),
			frame_rate(frame_rate) {
		this->frame_data = std::vector<int>(list);
		this->size = int(frame_data.size());
	};

	~SpriteAnimation() {
	}

	std::string name;
	int frame_rate; // in S
	std::vector<int> frame_data;
	int size;
};

class SpriteAnimator {
public:
	SpriteAnimator(){};
	~SpriteAnimator();
	void add_animation(std::string name, int frame_rate, std::initializer_list<int> list);
	void delete_animation(const std::string &name);
	void play_animation(const std::string &name);
	void pause();
	void update(int delta_time);
	int current_frame();

private:
	std::map<std::string, std::unique_ptr<SpriteAnimation>> animations;
	SpriteAnimation *current_animation_ = nullptr;
	bool is_animation_playing_ = false;
	int current_frame_ = 0;
	int frame_counter_ = 0;
};

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(SDL_Renderer &renderer, const std::string &path, int width, int height, int rows, int columns);
	~AnimatedSprite();
	void update(int delta_time);
	SpriteAnimator *animator() { return animator_.get(); }

private:
	std::unique_ptr<SpriteAnimator> animator_;
	int rows, columns;
};
} // namespace sl2dge
