#include "ItemList.h"

void Items::InitItems(Game* game) {
	item_list = new std::vector<std::unique_ptr<Item>>();
	item_list->push_back(
		std::make_unique<Item>(
			0,
			"Letter for West House",
			"A letter for West House",
			std::make_unique<Sprite>(*game->renderer(), "../resources/images/items/letter.png", 16, 16)
			)
	);
	item_list->push_back(
		std::make_unique<Item>(
			1,
			"Letter for South House",
			"A letter for South House",
			std::make_unique<Sprite>(*game->renderer(), "../resources/images/items/letter.png", 16, 16)
			)
	);
	item_list->push_back(
		std::make_unique<Item>(
			2,
			"Letter for East House",
			"A letter for East House",
			std::make_unique<Sprite>(*game->renderer(), "../resources/images/items/letter.png", 16, 16)
			)
	);
	item_list->push_back(
		std::make_unique<Item>(
			3,
			"Letter for Center House",
			"A letter for Center House",
			std::make_unique<Sprite>(*game->renderer(), "../resources/images/items/letter.png", 16, 16)
			)
	);
}

void Items::ClearItems() {

	item_list->clear();
	delete item_list;

}

Item* Items::GetItem(const int id) {

	for (auto it = item_list->begin(); it != item_list->end(); ++it) {

		if ((*it)->id() == id)
			return (*it).get();
	}
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Item with ID %d doesn't exist", id);
	return nullptr;
}
