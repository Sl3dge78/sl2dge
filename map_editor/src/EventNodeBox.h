#pragma once

#include <memory>
#include <string>
#include <vector>

//#include "GameEvent.h"
#include "SDL/SDL.h"

#include "Point.h"
#include "Game.h"

using namespace sl2dge;

class EventNodeBox  {
public:
	EventNodeBox(int out_amt = 1);

	SDL_Rect in_plug();
	SDL_Rect out_plug(int i = 0);
	void set_out_amt(int i);

	SDL_Rect rect = { 0, 0, 100, 100 };
	int id() const { return id_; }

	std::vector<int> next;
	static int next_id;

	void draw(Game* game);
	void handle_events(Game* game, const SDL_Event& e);
	void resize(int x, int y);

	int is_point_in_plug(SDL_Point* point);

	bool has_prev = false;

protected:
	std::string title = "";
	bool has_in = true;

private:
	
	SDL_Rect get_corner() const { return SDL_Rect{ rect.x + rect.w - 5, rect.y + rect.h - 5, 5, 5 };	}

	int id_;
	bool is_resizing_ = false;
	bool is_moving_ = false;
	int out_amt_ = 1;


	//GameEvent::EventTypes type = GameEvent::EventTypes::RandomBranch;
};

class DialogNodeBox : public EventNodeBox {
public:
	DialogNodeBox() : EventNodeBox(2) {
		title = "Dialog";
	}

	std::string text = "";

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