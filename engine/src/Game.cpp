#include "Game.h"

#include "GameState.h"
#include "SceneManager.h"

namespace sl2dge {

	Game::Game(const std::string window_name, const int window_width, const int window_heigth) {
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);
		
		this->window_ = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_heigth, SDL_WINDOW_SHOWN);
		this->renderer_ = SDL_CreateRenderer(this->window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		this->font_ = FC_CreateFont();
		FC_LoadFont(font_, renderer_, font_path_.c_str(), font_size_, SDL_Color{ 0, 0, 0, 255 }, TTF_STYLE_NORMAL);

		quest_manager_ = std::make_unique<QuestManager>();

		//SceneManager::Init();

	}

	Game::~Game() {
		while (!states_.empty()) {
			states_.pop_back();
		}

		//SceneManager::Exit();

		FC_FreeFont(font_);


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
			}
		}
	}

	// Destructor is called on previous state
	void Game::change_state(std::unique_ptr<GameState> state) {
		if (!states_.empty()) {
			states_.pop_back();
		}
		states_.push_back(std::move(state));
		states_.back()->start(this);
	}

	void Game::push_state(std::unique_ptr<GameState> state) {
		
		states_.back()->on_state_pause();
		states_.push_back(std::move(state));
		states_.back()->start(this);
	}

	void Game::pop_state() {
		if (!states_.empty()) {
			states_.pop_back();
		}

		if (!states_.empty()) {
			states_.back()->on_state_resume();
		}
	}
}