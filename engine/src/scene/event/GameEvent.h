#pragma once

#include <memory>
#include <string>

#include <utility>
#include <vector>

#include <SDL/SDL.h>

#include "addons/guid.h"
#include "addons/pugixml.hpp"
#include "core/GameState.h"
#include "math/Point.h"

namespace sl2dge {

class Game;
class EventChain;

class GameEvent {
public:
	static std::unique_ptr<GameEvent> create_event(const pugi::xml_node &node);

	virtual ~GameEvent() = default;

	virtual void get_xml_data(pugi::xml_node &node);
	virtual void on_activate(Game *game, EventChain *gem) = 0;
	virtual void on_end(Game *game) = 0;

	virtual Guid next() const = 0;
	virtual Guid next(int i) const = 0;
	virtual void set_next(const Guid &id, int i = 0) = 0;
	virtual int next_amt() const = 0;

	Guid id() const { return id_; }

	/* EDITOR */
	virtual void draw(Game *game);
	virtual void handle_events(Game *game, const SDL_Event &e);

	SDL_Rect in_plug();
	SDL_Rect out_plug(int i = 0);

	int is_point_in_plug(SDL_Point *point);

	void resize(int x, int y) {
		rect_.w += x;
		if (rect_.w < 100)
			rect_.w = 100;
		rect_.h += y;
		if (rect_.h < 100)
			rect_.h = 100;
		on_box_moved();
	}
	void translate(int x, int y) {
		rect_.x += x;
		rect_.y += y;
		on_box_moved();
	}

	bool has_in() const { return has_in_; }
	bool has_prev = false;
	SDL_Rect get_corner() const { return SDL_Rect{ rect_.x + rect_.w - 5, rect_.y + rect_.h - 5, 5, 5 }; }
	const SDL_Rect *rect() const { return &rect_; }

protected:
	GameEvent(const Guid id = newGuid()) :
			id_(id){};
	GameEvent(const pugi::xml_node *node);

	/* EDITOR */
	std::string title = "";
	SDL_Rect rect_ = { 0, 0, 100, 100 };

private:
	Guid id_ = Guid();
	bool has_in_ = true;

	virtual void on_box_moved() {}
};

class Dialog : public GameEvent {
public:
	// Creates an event that will display a dialog. The first text will be displayed in the dialog box, the rest will be displayed as choices.
	// Each choice will trigger next_nodes in their respective order.
	// Upon triggering, this event will push a gamestate to the gamestate manager.
	Dialog(const pugi::xml_node &node);
	Dialog() { title = "Dialog"; };
	~Dialog(){};

	virtual void get_xml_data(pugi::xml_node &node);
	virtual void on_activate(Game *game, EventChain *gem) override;
	virtual void on_end(Game *game) override;

	virtual void draw(Game *game);
	virtual void handle_events(Game *game, const SDL_Event &e);
	Guid next() const override { return next_; }
	Guid next(int i) const override { return next_; }
	void set_next(const Guid &id, int i = 0) override { next_ = id; }
	int next_amt() const override { return 1; }

	std::string text = "";

private:
	bool is_editing_text = false;
	Guid next_ = Guid();
	SDL_Rect get_text_box() { return SDL_Rect{ rect_.x + 1, rect_.y + 32, rect_.w - 2, rect_.h - 32 - 5 }; }
};

class DialogChoice : public Dialog {
public:
	// Creates an event that will display a dialog. The first text will be displayed in the dialog box, the rest will be displayed as choices.
	// Each choice will trigger next_nodes in their respective order.
	// Upon triggering, this event will push a gamestate to the gamestate manager.
	DialogChoice(const pugi::xml_node &node);
	~DialogChoice(){};

	void set_choice(const int choice = 0) { this->choice_ = choice; }
	std::string *choice_text(int i) { return &choices_[i]; }

	Guid next() const override { return next_nodes_[choice_]; }
	Guid next(int i) const override { return next_nodes_[i]; }
	void set_next(const Guid &id, int i = 0) override { next_nodes_[i] = id; }

	int next_amt() const override { return choice_amt_; }

	virtual void on_activate(Game *game, EventChain *gem) override;
	virtual void on_end(Game *game) override;

	virtual void draw(Game *game);
	virtual void handle_events(Game *game, const SDL_Event &e);

private:
	int choice_amt_;
	std::vector<std::string> choices_;

	int choice_ = 0;
	std::vector<Guid> next_nodes_;
};

/*

	class Teleport : public GameEvent {
	public:
		// Creates a new event that will teleport an IMoveable obj to a specified position.
		Teleport(const Guid id, IMoveable* obj, const Point& pos, const Guid next = Guid());

		// Inherited via GameEvent
		virtual void on_activate(Game* game, EventChain* gem) override;
		virtual void on_end(Game* game) override {};

	private:
		Point pos;
		IMoveable* obj;
	};
	
	class SceneChange : public GameEvent {
	public:
		// Creates a new event that will change the current scene to scene_name. Don't forget to TP the player afterwards!
		SceneChange(const Guid id, std::unique_ptr <GameState> state, const Guid* next = nullptr) : GameEvent(id, next), state_(std::move(state)) {};
		virtual void on_activate(Game* game, EventChain* gem) override;
		virtual void on_end(Game* game) override {};	

	private:
		std::unique_ptr<GameState> state_ = nullptr;

	};

	class QuestStatus : public GameEvent {

	public:
		// Creates a new event that will change quest quest_id to new_status.
		QuestStatus(const Guid id, const int quest_id, const int new_status, const Guid next = Guid()) : GameEvent(id, next), quest_id_(quest_id), new_status_(new_status) {};

		virtual void on_activate(Game* game, EventChain* gem) override;
		virtual void on_end(Game* game) override {};

	private:
		int quest_id_;
		int new_status_;
	};

	class QuestEQBranch : public GameEvent {

	public:
		// Creates a new event that will jump to a specific event depending on the quest's status.
		QuestEQBranch(const Guid id, const int quest_id, const int status_to_check, const Guid next_if_true, const Guid next_if_false = Guid())
			: GameEvent(id), quest_id_(quest_id), status_to_check_(status_to_check), 
			next_if_true_(next_if_true), next_if_false_(next_if_false) {
		};

		virtual void on_activate(Game* game, EventChain* gem) override;
		virtual void on_end(Game* game) override {};

	private:
		Guid next_if_true_;
		Guid next_if_false_;
		int quest_id_;
		int status_to_check_;
	};

	class QuestGTBranch : public GameEvent {

	public:
		// Creates a new event that will jump to a specific event if status is over or equals status_to_check
		QuestGTBranch(const Guid id, const int quest_id, const int status_to_check, const Guid next_if_greater, const Guid next_if_lower = Guid())
			: GameEvent(id), quest_id_(quest_id), status_to_check_(status_to_check),
			next_if_greater_(next_if_greater), next_if_lower_(next_if_lower) {
		};

		virtual void on_activate(Game* game, EventChain* gem) override;
		virtual void on_end(Game* game) override {};

	private:
		Guid next_if_greater_;
		Guid next_if_lower_;
		int quest_id_;
		int status_to_check_;

	};

	class RandomBranch : public GameEvent {
	public:

		RandomBranch(const Guid id, std::vector<Guid> next_nodes) : GameEvent(id), next_nodes_(next_nodes) {};
		virtual void on_activate(Game* game, EventChain* gem) override;
		virtual void on_end(Game* game) override;

	private:
		std::vector<Guid> next_nodes_;
		
	};
	*/
} // namespace sl2dge
