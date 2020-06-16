#pragma once
#include <array>
#include <memory>

#include <sl2dge.h>

#include "ItemList.h"

using namespace sl2dge;

class Inventory {

private:

	struct ItemData {
		int id;
		int qty;
	};

	std::array<ItemData, 5*8> contents = std::array<ItemData, 5 * 8>();
	

public:
	Inventory() {
		for (auto& i : contents) {
			i = ItemData{ -1, 0 };
		}
	}
	~Inventory() {};
	int HasItem(int id) const;
	// Entering a negative qty will remove that amount
	void AddRemoveItem(int id, int qty = 1);
	// Qty param optional, if not specified will remove all
	void RemoveItem(int id, int qty = -1);
	Item* GetItem(const int i);

	void SortInventory();
	int Length() { return int(contents.size()); }
	
};

class InventoryAddEvent : public GameEvent {
public:
	InventoryAddEvent(Guid id, Inventory* inventory, int item_id, int qty, Guid next) : GameEvent(id, next), inventory(inventory), item_id(item_id), qty(qty) {};

	virtual void on_activate(Game* game, GameEventManager* gem) override;
	virtual void on_end(Game* game) override {};

private:
	Inventory* inventory;
	int item_id;
	int qty;
};

class ConditionalInventoryEvent : public GameEvent {
public:
	ConditionalInventoryEvent(const Guid id, Inventory* inventory, const int item_id, const int item_qty, const Guid id_true, const Guid id_false) :
		GameEvent(id, id_false), inventory(inventory), item_id(item_id), item_qty(item_qty), id_true(id_true), id_false(id_false) {
	}
	virtual void on_activate(Game* game, GameEventManager* gem) override;
	virtual void on_end(Game* game) override {};

private:
	Inventory* inventory;
	int item_id;
	int item_qty;
	Guid id_true;
	Guid id_false;
};