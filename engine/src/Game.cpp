#include "Game.h"

#include "SceneManager.h"

namespace sl2dge {

	Game::Game(const std::string window_name, const Uint32 window_width, const Uint32 window_height) {
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);
		window_width_ = window_width;
		window_height_ = window_height;
		this->window_ = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
		this->renderer_ = SDL_CreateRenderer(this->window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		this->black_font_ = FC_CreateFont();
		FC_LoadFont(black_font_, renderer_, font_path_.c_str(), font_size_, SDL_Color{ 0, 0, 0, 255 }, TTF_STYLE_NORMAL);
		this->white_font_ = FC_CreateFont();
		FC_LoadFont(white_font_, renderer_, font_path_.c_str(), font_size_, SDL_Color{ 255, 255, 255, 255 }, TTF_STYLE_NORMAL);
		
		quest_manager_ = std::make_unique<QuestManager>();

		//SceneManager::Init();

	}

	Game::~Game() {
		while (!states_.empty()) {
			states_.back()->on_state_exit(this);
			states_.pop_back();
		}

		//SceneManager::Exit();

		FC_FreeFont(black_font_);
		FC_FreeFont(white_font_);

		TTF_Quit();

		SDL_DestroyRenderer(this->renderer_);
		SDL_DestroyWindow(this->window_);

		IMG_Quit();
		SDL_Quit();
	}

	int Game::loop() {
		SDL_Event event;
		int last_time = SDL_GetTicks();

		while (1) {
			// Update time
			int time = SDL_GetTicks();
			delta_time_ = time - last_time;

			if (!wait_for_vsync_ || delta_time_ >= max_frame_time_) {

				last_time = time;

				while (SDL_PollEvent(&event) == 1) {
					switch (event.type) {
					case SDL_QUIT:
						return 0;
					default:
						states_.back()->handle_events(this, event); // Only the top state reads input
					}
				}

				states_.back()->input(this); // Only the top state reads input

				for (auto&& it : states_) {
					it->update(this);
				}

				SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(renderer_);
				for (auto&& it : states_) {
					it->draw(this);
				}
				SDL_RenderPresent(renderer_);

				if (pop_state_next_frame) {
					pop_state_();
					pop_state_next_frame = false;
				}

			}
		}
	}

	// Destructor is called on previous state
	void Game::change_state(std::unique_ptr<GameState> state) {
		if (!states_.empty()) {
			states_.back()->on_state_exit(this);
			states_.pop_back();
		}
		states_.push_back(std::move(state));
		states_.back()->start(this);
	}

	void Game::push_state(std::unique_ptr<GameState> state) {
		
		states_.back()->on_state_pause(this);
		states_.push_back(std::move(state));
		states_.back()->start(this);
	}

	void Game::pop_state() {
		pop_state_next_frame = true;
	}

	void Game::pop_state_() {
		if (!states_.empty()) {
			states_.back()->on_state_exit(this);
			states_.pop_back();
		}

		if (!states_.empty()) {
			states_.back()->on_state_resume(this);
		}
	}
}