#pragma once

#include <memory>
#include <string>
#include <vector>

//#include "GameEvent.h"
#include "SDL/SDL.h"

#include "sl2dge.h"
#include "UI.h"

using namespace sl2dge;

class EventNodeBox  {
public:
	EventNodeBox(int out_amt = 1);

	SDL_Rect in_plug();
	SDL_Rect out_plug(int i = 0);
	void set_out_amt(int i);

	Guid guid() const { return uuid_; }

	std::vector<Guid> next;

	virtual void draw(Game* game);
	virtual void handle_events(Game* game, const SDL_Event& e);
	void resize(int x, int y);

	int is_point_in_plug(SDL_Point* point);
	bool has_in() const { return has_in_; }

	bool has_prev = false;

protected:
	std::string title = "";
	bool has_in_ = true;
	SDL_Rect rect = { 0, 0, 100, 100 };

	virtual void on_box_moved() {}

private:
	
	SDL_Rect get_corner() const { return SDL_Rect{ rect.x + rect.w - 5, rect.y + rect.h - 5, 5, 5 };	}

	Guid uuid_;
	bool is_resizing_ = false;
	bool is_moving_ = false;
	int out_amt_ = 1;


	//GameEvent::EventTypes type = GameEvent::EventTypes::RandomBranch;
};


/* TRIGGER */
class TriggerBox : public EventNodeBox {
public:
	TriggerBox() : EventNodeBox(1) {
		title = "Entry";
		has_in_ = false;
		on_box_moved();
	}

	void draw(Game* game) override;
	void handle_events(Game* game, const SDL_Event& e) override;

protected:
	void on_box_moved() override;

private : 

	SDL_Point map_pos;
	ToggleBox interactable_ = ToggleBox("Inter", false);
	ToggleBox is_in_place_ = ToggleBox("In Place", true); //  you need to stand on the item to trigger the event
	ToggleBox activate_once_ = ToggleBox("Once", false); // If true, won't activate ever again
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

	SDL_Rect get_text_box() { return SDL_Rect{ rect.x + 1, rect.y + 32, rect.w - 2, rect.h - 32 - 5 }; }

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