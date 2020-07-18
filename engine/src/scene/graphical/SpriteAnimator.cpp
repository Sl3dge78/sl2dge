#include "SpriteAnimator.h"

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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

SpriteAnimatorSystem::SpriteAnimatorSystem() {
	this->add_component_filter<Sprite>();
	this->add_component_filter<SpriteAnimator>();
	this->set_filter_type(FilterType::FILTER_AND);
}

void SpriteAnimatorSystem::update(const int delta_time) {
	for (auto e : entities_) {
		auto animator = e->get_component<SpriteAnimator>();
		if (animator->is_animation_playing()) {
			animator->frame_counter += delta_time;

			if (animator->frame_counter >= 1000 / animator->current_animation()->frame_rate) {
				animator->frame_counter = 0;
				animator->current_frame++;

				if (animator->current_frame >= animator->current_animation()->size) {
					animator->current_frame = 0;
				}

				auto sprite = e->get_component<Sprite>();
				sprite->src.x = (animator->frame_data() % sprite->columns) * sprite->src.w;
				sprite->src.y = (animator->frame_data() / sprite->columns) * sprite->src.h;
			}
		}
	}
}
} // namespace sl2dge
