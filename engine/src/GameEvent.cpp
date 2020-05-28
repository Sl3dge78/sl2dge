#include "GameEvent.h"

#include <memory>
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "DialogState.h"
#include "Components.h"
#include "GameState.h"

namespace sl2dge {

	Trigger::Trigger(const Point& position, const int event_id, const bool interactable_, const bool is_in_place_, const bool activate_once_) :
		position_(position), triggered_event_id_(event_id), interactable_(interactable_), is_in_place_(is_in_place_), activate_once_(activate_once_) {
	}

	int Trigger::on_trigger_activate() {
		if (!entered_ && !activated) {
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Event %d triggered", triggered_event_id_);
			if (!interactable_) {
				entered_ = true;
			}

			if (activate_once_) {
				activated = true;
			}

			return triggered_event_id_;
		}

		
		return -1;
	}

	void Trigger::reset() {
		if (!interactable_) {
			entered_ = false;
		}
	}

	/* DIALOG */
	
	Dialog::Dialog(int id, const std::string& text, int next) :
		GameEvent(id, next), text_(text) {
	}

	void Dialog::on_activate(Game* game, GameEventManager* gem) {
		game->push_state(std::make_unique<DialogState>(gem, this));
	}

	void Dialog::on_end(Game* game) {
		game->pop_state();
	}

	/* CHOICE */

	DialogChoice::DialogChoice(int id, std::string text, std::vector<std::string> choices, std::vector<int> next_nodes) :
		Dialog(id,text, next_nodes_[0]), choices_(choices) {
	}

	void DialogChoice::on_activate(Game* game, GameEventManager* gem) {
		game->push_state(std::make_unique<DialogState>(gem, this));
	}

	void DialogChoice::on_end(Game* game) {
		game->pop_state();
	}

	void DialogChoice::set_choice(const int choice) {
		this->choice_ = choice;
		next = next_nodes_[choice];
	}


	/* TELEPORT */

	Teleport::Teleport(const int id, IMoveable* obj, const Point& pos, const int next)
		: GameEvent(id, next), pos(pos), obj(obj){
	}
	void Teleport::on_activate(Game* game, GameEventManager* gem) {
		obj->teleport_to(Vector2(float(pos.x * 16), float(pos.y * 16)));
		gem->next_event();
	}

	/* SCENE CHANGE */

	void SceneChange::on_activate(Game* game, GameEventManager* gem) {
		// SceneManager::LoadScene(game, this->scene_name);
		game->change_state(std::move(state_));
		//gem->next_event();
	}

	/* QUEST  */

	void QuestStatus::on_activate(Game* game, GameEventManager* gem) {
		game->quest_manager()->change_quest_status(quest_id_, new_status_);
		SDL_Log("Quest %d has progressed to step %d!", quest_id_, new_status_);
		gem->next_event();
	}

	/* CONDITIONAL QUEST */

	void QuestEQBranch::on_activate(Game* game, GameEventManager* gem) {
		auto status = game->quest_manager()->quest(quest_id_)->status;
		this->next = status == status_to_check_ ? next_if_true_ : next_if_false_;
		SDL_Log("Checking if Quest %d is at step %d. Quest is at %d, jumping to event %d.", quest_id_, status_to_check_, status, this->next);
		gem->next_event();

	}

	// Inherited via GameEvent

	void QuestGTBranch::on_activate(Game* game, GameEventManager* gem) {
		auto status = game->quest_manager()->quest(quest_id_)->status;
		this->next = status >= status_to_check_ ? next_if_greater_ : next_if_lower_;
		SDL_Log("Checking if Quest %d is at least above step %d. Quest is at %d, jumping to event %d.", quest_id_, status_to_check_, status, this->next);
		gem->next_event();
	}

	void RandomBranch::on_activate(Game* game, GameEventManager* gem) {
		
		srand(unsigned int(time(NULL))); // TODO : move that elsewhere

		auto val = rand() % this->next_nodes_.size();
		this->next = next_nodes_[val];
		gem->next_event();

	}

	void RandomBranch::on_end(Game* game) {

	}
}