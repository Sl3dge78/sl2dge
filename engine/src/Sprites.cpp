#include "Sprites.h"

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Camera.h"
#include "Rect.h"

namespace sl2dge {

#pragma region Sprite

	Sprite::Sprite(SDL_Renderer& renderer, const std::string& path, int width, int height) {
		SDL_Surface* surf = IMG_Load(path.c_str());
		if (surf == NULL) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error reading file %s : %s", path, IMG_GetError());
			throw std::runtime_error(IMG_GetError());
		}

		texture = SDL_CreateTextureFromSurface(&renderer, surf);
		src = { 0, 0, width, height };
		SDL_FreeSurface(surf);

	}


	Sprite::~Sprite() {
		SDL_DestroyTexture(texture);
	}

	//Draw relative to camera
	void Sprite::draw(SDL_Renderer& renderer, const Camera& cam, const Point& position) {
		RenderCopyWorld(renderer, *texture, cam, &src, &Rect(position.x, position.y, src.w, src.h));
	}

	void Sprite::draw(SDL_Renderer& renderer, const Camera& cam, const Rect& position) {
		RenderCopyWorld(renderer, *texture, cam, &src, &Rect(position.x, position.y, src.w, src.h));
	}

	//Draw in absolute screen position
	void Sprite::draw_absolute(SDL_Renderer& renderer, const Rect& position) {
		RenderCopy(renderer, *texture, &src, &position);
	}

#pragma endregion

#pragma region AnimatedSprite

	AnimatedSprite::AnimatedSprite(SDL_Renderer& renderer, const std::string& path, int width, int height, int columns, int rows)
		: Sprite(renderer, path, width, height), columns(columns), rows(rows) {

		animator_ = std::make_unique<SpriteAnimator>();
	}

	AnimatedSprite::~AnimatedSprite() {
	}

	void AnimatedSprite::update(int delta_time) {
		animator_->update(delta_time);
		auto current_frame_ = animator_->current_frame();
		src.x = (animator_->current_frame() % columns) * src.w;
		src.y = (animator_->current_frame() / columns) * src.h;
	}

#pragma endregion

#pragma region SpriteAnimator

	SpriteAnimator::~SpriteAnimator() {
		//animations.clear();
	}

	void SpriteAnimator::add_animation(std::string name, int frame_rate, std::initializer_list<int> list) {
		
		auto a = std::unique_ptr<SpriteAnimation>(std::make_unique<SpriteAnimation>(name, frame_rate, list));		
		animations.emplace(name, std::move(a));
	}

	void SpriteAnimator::delete_animation(const std::string& name) {
		if (current_animation_->name == name) {
			pause();
			current_animation_ = nullptr;
		}
		animations.erase(name);
	}

	void SpriteAnimator::play_animation(const std::string& name) {
		if (current_animation_ != nullptr && current_animation_->name == name)
			return;

		if (animations.find(name) == animations.end()) {
			throw std::runtime_error("Unable to find animation");
			return;
		}

		current_animation_ = animations.find(name)->second.get();
		is_animation_playing_ = true;
		current_frame_ = 0;
		frame_counter_ = 0;
	}

	void SpriteAnimator::pause() {
		is_animation_playing_ = false;
	}

	void SpriteAnimator::update(int delta_time) {

		if (current_animation_ != nullptr) {
			if (is_animation_playing_) {
				frame_counter_ += delta_time;
			}

			if (frame_counter_ >= 1000 / current_animation_->frame_rate) {
				frame_counter_ = 0;
				current_frame_++;
				if (current_frame_ >= current_animation_->size) {
					current_frame_ = 0;
				}
			}
		}
	}

	int SpriteAnimator::current_frame() {
		if (current_animation_ != nullptr) {
			return current_animation_->frame_data.at(current_frame_);
		} else {
			return 0;
		}
	}

#pragma endregion

}