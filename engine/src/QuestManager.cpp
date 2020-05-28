#include "QuestManager.h"

#include "SDL\SDL.h"

sl2dge::QuestManager::QuestManager() {
}

sl2dge::QuestManager::~QuestManager() {
	quest_list_.clear();
}

void sl2dge::QuestManager::add_game_quest(Quest* q) {
	quest_list_.push_back(std::unique_ptr<Quest>(q));
	SDL_Log("Quest %d added!", q->id);
}

Quest* sl2dge::QuestManager::quest(const int id) {
	for (auto& q : quest_list_) {
		if (q->id == id) {
			return q.get();
		}
	}
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Couldn't find quest id %d", id);
	return nullptr;

}

void sl2dge::QuestManager::change_quest_status(const int id, const int status) {
	quest(id)->status = status;
	// TODO : SAVE TO FILE
}
