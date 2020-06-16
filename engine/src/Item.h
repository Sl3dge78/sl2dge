#pragma once

#include "Sprites.h"
#include <string>

namespace sl2dge {

class Item {
private:
	int id_;
	std::string name_;
	std::string description_;
	std::unique_ptr<Sprite> sprite_;

public:
	Item(int id, const std::string name, const std::string description, std::unique_ptr<Sprite> sprite) :
			id_(id), name_(name), description_(description), sprite_(std::move(sprite)) {}
	~Item(){};

	std::string *name() { return &name_; }
	int id() { return id_; }
	std::string *description() { return &description_; }
	Sprite *sprite() { return sprite_.get(); }

	friend bool operator==(const Item &a, const Item &b) {
		return a.id_ == b.id_;
	}
};
} // namespace sl2dge
