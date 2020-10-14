#include "SpriteAnimator.h"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "core/Game.h"
#include "scene/graphical/Sprite.h"

namespace sl2dge {
void SpriteAnimator::add_animation(std::string name, unsigned int frame_rate, std::initializer_list<int> list) {
	auto a = std::unique_ptr<SpriteAnimation>(std::make_unique<SpriteAnimation>(name, frame_rate, list));
	animations.emplace(name, std::move(a));
}

void SpriteAnimator::delete_animation(const std::string &name) {
	if (current_animation_->name == name) {
		pause();
		current_animation_ = nullptr;
	}
	animations.erase(name);
}

void SpriteAnimator::play_animation(const std::string &name) {
	if (current_animation_ != nullptr && current_animation_->name == name)
		return;

	if (animations.find(name) == animations.end()) {
		throw std::runtime_error("Unable to find animation");
		return;
	}

	current_animation_ = animations.find(name)->second.get();
	is_animation_playing_ = true;
	current_frame = 0;
	frame_counter = 0;
}

void SpriteAnimator::pause() {
	is_animation_playing_ = false;
}

inline bool SpriteAnimator::is_animation_playing() {
	return current_animation_ != nullptr && is_animation_playing_;
}

inline SpriteAnimation *SpriteAnimator::current_animation() const {
	return current_animation_;
}

void SpriteAnimator::update(Game *game) {
	if (is_animation_playing()) {
		frame_counter += game->delta_time();

		if (frame_counter >= 1000 / current_animation()->frame_rate) {
			frame_counter = 0;
			current_frame++;

			if (current_frame >= current_animation()->size) {
				current_frame = 0;
			}

			auto sprite = entity()->get_component<Sprite>();
			sprite->src.x = (frame_data() % sprite->columns()) * sprite->src.w;
			sprite->src.y = (frame_data() / sprite->columns()) * sprite->src.h;
		}
	}
}
} // namespace sl2dge
