#pragma once

#include <memory>

#include "Quest.h"

namespace sl2dge {

class QuestManager {
public:
	QuestManager();
	~QuestManager();

	// Adds a quest to the list
	// See Quest class constructor for more details.
	void add_game_quest(Quest *q);
	Quest *quest(const int id);
	void change_quest_status(const int id, const int status);

private:
	std::vector<std::unique_ptr<Quest>> quest_list_;
};

} // namespace sl2dge
