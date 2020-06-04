#pragma once

#include <memory>
#include <string>

#include <vector>
#include <utility>

#include "SDL/SDL.h"

#include "Point.h"
#include "Components.h"
#include "guid.h"

namespace sl2dge {
	
	class Game;
	class GameState;
	class GameEventManager;

	class GameEvent {

	public:
		/*
		enum class EventTypes {
			Dialog = 0,
			DialogChoice = 1,
			Teleport = 2,
			SceneChange = 3,
			QuestStatus = 4, 
			QuestEQBranch = 5,
			QuestGTBranch = 6,
			RandomBranch = 7,
		};*/

		Guid GetID() const { return id; }
		Guid GetNext() const { return next; }
		virtual ~GameEvent() = default;
		virtual void on_activate(Game* game, GameEventManager* gem) = 0;
		virtual void on_end(Game* game) = 0;

	protected:
		 GameEvent(const Guid id, const Guid next = Guid()) :id(id), next(next) {};
		 Guid next;

	private:
		Guid id;

	};

	
	class Dialog : public GameEvent {
	public:
		// Creates an event that will display a dialog. The first text will be displayed in the dialog box, the rest will be displayed as choices.
		// Each choice will trigger next_nodes in their respective order.
		// Upon triggering, this event will push a gamestate to the gamestate manager.
		Dialog(Guid id, const std::string& text, Guid next);
		~Dialog() {};
		
		virtual void on_activate(Game* game, GameEventManager* gem) override;
		virtual void on_end(Game* game) override;
		std::string* text() { return &text_; }

	private:
		std::string text_;
	};

	class DialogChoice : public Dialog {
	public:
		// Creates an event that will display a dialog. The first text will be displayed in the dialog box, the rest will be displayed as choices.
		// Each choice will trigger next_nodes in their respective order.
		// Upon triggering, this event will push a gamestate to the gamestate manager.
		DialogChoice(Guid id, std::string text, std::vector<std::string> choices, std::vector<Guid> next_nodes);
		~DialogChoice() {};
		
		void set_choice(const int choice = 0);
		int choice_amnt() { return int(choices_.size()); }
		std::string* choice_text(int i) { return &choices_[i]; }

		virtual void on_activate(Game* game, GameEventManager* gem) override;
		virtual void on_end(Game* game) override;

	private:
		std::vector<std::string> choices_;
		int choice_ = 0;
		std::vector<Guid> next_nodes_;
	};

	class Teleport : public GameEvent {
	public:
		// Creates a new event that will teleport an IMoveable obj to a specified position.
		Teleport(const Guid id, IMoveable* obj, const Point& pos, const Guid next = Guid());

		// Inherited via GameEvent
		virtual void on_activate(Game* game, GameEventManager* gem) override;
		virtual void on_end(Game* game) override {};

	private:
		Point pos;
		IMoveable* obj;
	};
	
	class SceneChange : public GameEvent {
	public:
		// Creates a new event that will change the current scene to scene_name. Don't forget to TP the player afterwards!
		SceneChange(const Guid id, std::unique_ptr <GameState> state, const Guid next = Guid()) : GameEvent(id, next), state_(std::move(state)) {};
		virtual void on_activate(Game* game, GameEventManager* gem) override;
		virtual void on_end(Game* game) override {};	

	private:
		std::unique_ptr<GameState> state_ = nullptr;

	};

	class QuestStatus : public GameEvent {

	public:
		// Creates a new event that will change quest quest_id to new_status.
		QuestStatus(const Guid id, const int quest_id, const int new_status, const Guid next = Guid()) : GameEvent(id, next), quest_id_(quest_id), new_status_(new_status) {};

		virtual void on_activate(Game* game, GameEventManager* gem) override;
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

		virtual void on_activate(Game* game, GameEventManager* gem) override;
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

		virtual void on_activate(Game* game, GameEventManager* gem) override;
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
		virtual void on_activate(Game* game, GameEventManager* gem) override;
		virtual void on_end(Game* game) override;

	private:
		std::vector<Guid> next_nodes_;
		
	};

	class Trigger {
	public:
		//Creates a new event trigger. When conditions are met, this will trigger event triggered_event_id.
		// Params: 
		// interactable: does the player need to press the action button to trigger the event?
		// is_in_place: true : the player will have to be at the position specified to trigger the event. false : looking at it will trigger it. Note : this only works on interactable = true.
		// activate_once : does the trigger trigger once?
		Trigger(const Point& position, const Guid triggered_event_id, const bool interactable_ = false, const bool is_in_place_ = true, const bool activate_once_ = false);

		Guid on_trigger_activate();
		void reset();

		bool is_interactable() const { return interactable_; }
		bool is_in_place() const { return is_in_place_; }
		bool is_once() const { return activate_once_; }
		bool is_activated() const { return entered_; }
		Point position() const { return position_; }

	private:
		Guid triggered_event_id_;
		bool interactable_ = false; // you need to activate it
		bool is_in_place_ = true; //  you need to stand on the item to trigger the event
		bool activate_once_ = false; // If true, won't activate ever again

		Point position_;
		bool entered_ = false;
		bool activated = false;
	};
}