#include "Inventory.h"

#include "SDL/SDL.h"

#include "Player.h"


int Inventory::HasItem(int id) const {
	// Check if inventory doesn't already have the item
	for (auto& i : contents) {
		if (i.id == id) {
			return i.qty;
		}
	}
	return 0;
}

void Inventory::AddRemoveItem(int id, int qty) {
	for (auto& i : contents) {
		if (i.id == id) {
			i.qty += qty;
			if (i.qty <= 0) {
				i.id = -1;
				i.qty = 0;
			}
			SortInventory();
			return;
		} else if (qty > 0 && i.id == -1) { // Empty slot
			i.id = id;
			i.qty = qty;
			return;
		}
	}
}

void Inventory::RemoveItem(int id, int qty) {
	for (auto& i : contents) {
		if (i.id == id) {
			i.qty -= qty;
			if (i.qty <= 0) {
				i.id = -1;
				i.qty = 0;
			}
			SortInventory();
			return;
		}
	}
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Trying to remove item %d, but none has been found in inventory!", id);
}

Item* Inventory::GetItem(const int i) {
	auto id = contents[i].id;
	if (id == -1)
		return nullptr;
	else
		return Items::GetItem(id);
}

void Inventory::SortInventory() {

	std::sort(contents.begin(), contents.end(), [](const ItemData& a, const ItemData& b) { return a.id < b.id; });

}
/*
void InventoryAddEvent::on_activate(Game* game, GameEventManager* gem) {
	inventory->AddRemoveItem(item_id, qty);
	gem->next_event();
}



void ConditionalInventoryEvent::on_activate(Game* game, GameEventManager* gem) {

	if (inventory->HasItem(item_id) >= item_qty)
		next_ = id_true;
	else
		next_ = id_false;
	gem->next_event();

}


*/