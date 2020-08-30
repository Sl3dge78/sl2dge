#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL/SDL.h>

#include "ECS/ECS.h"

namespace sl2dge {

struct SpriteAnimation {
public:
	SpriteAnimation(std::string name, int frame_rate, std::initializer_list<int> list) :
			name(name),
			frame_rate(frame_rate) {
		this->frame_data = std::vector<int>(list);
		this->size = int(frame_data.size());
	};

	std::string name;
	unsigned int frame_rate; // in S
	std::vector<int> frame_data;
	unsigned int size;
};

class SpriteAnimator : public Component {
public:
	void add_animation(std::string name, unsigned int frame_rate, std::initializer_list<int> list);
	void delete_animation(const std::string &name);
	void play_animation(const std::string &name);
	void pause();
	void update(Game *game) override;
	bool is_animation_playing();
	SpriteAnimation *current_animation() const;

	unsigned int current_frame = 0;
	unsigned int frame_counter = 0;

	unsigned int frame_data() const {
		if (current_animation_ != nullptr) {
			return current_animation_->frame_data.at(current_frame);
		} else {
			return 0;
		}
	}

private:
	std::map<std::string, std::unique_ptr<SpriteAnimation>> animations;
	SpriteAnimation *current_animation_ = nullptr;
	bool is_animation_playing_ = false;
};
} // namespace sl2dge
