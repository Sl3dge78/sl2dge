#pragma once

#include <memory>
#include <string>
#include <vector>

//#include "GameEvent.h"
#include "SDL/SDL.h"

#include "sl2dge.h"
#include "UI.h"

using namespace sl2dge;

class EventNodeBox {
public:
	virtual ~EventNodeBox() = default;

	Guid guid() const { return uuid_; }

	SDL_Rect in_plug();
	SDL_Rect out_plug(int i = 0);
	void set_out_amt(int i);
	int is_point_in_plug(SDL_Point* point);
	bool has_in() const { return has_in_; }
	bool has_prev = false;

	std::vector<Guid> next;

	virtual void draw(Game* game);
	virtual void handle_events(Game* game, const SDL_Event& e);

	void resize(int x, int y);
	void translate(int x, int y);
	SDL_Rect get_corner() const { return SDL_Rect{ rect_.x + rect_.w - 5, rect_.y + rect_.h - 5, 5, 5 }; }
	const SDL_Rect* rect() const { return &rect_; }

protected:
	EventNodeBox(int out_amt = 1);

	std::string title = "";
	bool has_in_ = true;
	SDL_Rect rect_ = { 0, 0, 100, 100 };

	virtual void on_box_moved() {}

private:
	Guid uuid_;
	int out_amt_ = 1;
};


/* TRIGGER */
class TriggerBox : public EventNodeBox {
public:
	TriggerBox();

	void draw(Game* game) override;
	void handle_events(Game* game, const SDL_Event& e) override;

protected:
	void on_box_moved() override;

private:

	SDL_Point map_pos;
	std::unique_ptr<ToggleBox> interactable_ = nullptr;
	std::unique_ptr<ToggleBox> is_in_place_ = nullptr; //  you need to stand on the item to trigger the event
	std::unique_ptr<ToggleBox> activate_once_ = nullptr; // If true, won't activate ever again
};

class DialogNodeBox : public EventNodeBox {
public:
	DialogNodeBox() : EventNodeBox(2) {
		title = "Dialog";
	}

	std::string text = "";

	void draw(Game* game);
	void handle_events(Game* game, const SDL_Event& e) override;

private:
	bool is_editing_text = false;

	SDL_Rect get_text_box() { return SDL_Rect{ rect_.x + 1, rect_.y + 32, rect_.w - 2, rect_.h - 32 - 5 }; }

};

class DialogChoiceNodeBox : public DialogNodeBox {
public:
	std::vector<std::string> choices;

	void AddChoice() {
		choices.push_back("");
	}

};

class RandomBranchBox : public EventNodeBox {
public:
	std::vector<int> next_nodes;

};