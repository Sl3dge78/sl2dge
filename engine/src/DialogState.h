#pragma once

#include "SDL/SDL_ttf.h"

#include "EventChain.h"
#include "Game.h"
#include "GameEvent.h"
#include "GameState.h"
#include "Point.h"
#include "Rect.h"

namespace sl2dge {

class DialogState : public GameState {
public:
	DialogState(EventChain *event_chain, Dialog *dialog) :
			current_dialog(dialog), event_chain(event_chain){};
	DialogState(EventChain *event_chain, DialogChoice *dialog) :
			current_dialog(dialog), current_choice_dialog(dialog), event_chain(event_chain) {
		is_choice = true;
	};
	~DialogState() {
		SDL_DestroyTexture(dialog_box_);
		SDL_DestroyTexture(selection_texture_);
	}

	// Inherited via GameState
	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, const SDL_Event &e) override;
	virtual void update(Game *game) override;
	virtual void input(Game *game) override;
	virtual void draw(Game *game) override;

private:
	const Rect single_rect = Rect(0, 224, 320, 64);
	const Point choice_anchor = Point(320, 224); // Anchor for multiple choice
	Rect choice_rect = Rect(choice_anchor.x - 64, choice_anchor.y - 8, 64, 8);

	void draw_dialog_box(SDL_Renderer &renderer, SDL_Texture &dialog_box, const int tile_size, const Rect &rect);
	SDL_Texture *dialog_box_ = nullptr;
	const std::string box_path = "resources/images/ui/dialogbox.png";

	Dialog *current_dialog = nullptr;
	DialogChoice *current_choice_dialog = nullptr;
	EventChain *event_chain = nullptr;

	std::vector<Rect> text_rects;
	std::vector<SDL_Texture *> current_textures;

	// Selection
	SDL_Texture *selection_texture_ = nullptr;
	const std::string selection_icon_path = "resources/images/ui/selection.png";
	int current_choice = 0;
	bool is_choice = false;

	virtual void on_state_resume(Game *game) override;
	virtual void on_state_pause(Game *game) override;
	virtual void on_state_exit(Game *game) override{};
};
} // namespace sl2dge
