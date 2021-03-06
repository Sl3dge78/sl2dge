#pragma once
#include <memory>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "addons/SDL_FontCache.h"

#include "GameState.h"

namespace sl2dge {

class Entity;

class Game {
public:
	enum class Platform {
		Editor,
		Runtime
	};

	// Creates a new game. Call loop to start the Game.
	Game(const std::string window_name = "",
			const Uint32 window_width = 800,
			const Uint32 window_heigth = 600, Platform platform = Platform::Runtime);
	~Game();

	// Will start the game's main loop.
	int loop();

	SDL_Renderer *renderer() { return renderer_; }

	Platform current_platform() { return platform_; }

	unsigned int window_width() { return window_width_; }
	unsigned int window_height() { return window_height_; }

	int delta_time() { return delta_time_; }
	void set_target_FPS(float val) {
		target_FPS_ = val;
		max_frame_time_ = 1.0f / target_FPS_;
	}

	FC_Font *black_font() { return black_font_; }
	FC_Font *font() { return black_font_; }
	FC_Font *white_font() { return white_font_; }

	GameState *current_state() const { return states_.back().get(); };
	void change_state(std::unique_ptr<GameState> state);
	void push_state(std::unique_ptr<GameState> state);
	void pop_state();

private:
	SDL_Renderer *renderer_;
	SDL_Window *window_;

	Platform platform_;

	Uint32 window_width_;
	Uint32 window_height_;

	FC_Font *black_font_;
	FC_Font *white_font_;
	const std::string font_path_ = "resources/fonts/unifont.ttf";
	const int font_size_ = 16;

	int delta_time_ = 0;
	bool wait_for_vsync_ = true;
	float target_FPS_ = 60.0;
	float max_frame_time_ = 1.0f / target_FPS_;

	std::vector<std::unique_ptr<GameState>> states_;

	void pop_state_();
	bool pop_state_next_frame = false;
};
} // namespace sl2dge
