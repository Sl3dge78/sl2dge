#pragma once

#include <vector> 

#include "sl2dge.h"

#include "EventNodeBox.h"

using namespace sl2dge;

class EventEditor : public GameState {
	public:

	// Inherited via GameState
	virtual void start(Game* game) override;
	virtual void handle_events(Game* game, const SDL_Event& e) override;
	virtual void input(Game* game) override;
	virtual void update(Game* game) override;
	virtual void draw(Game* game) override;
	virtual void on_state_resume() override; 
	virtual void on_state_pause() override;

private:
	std::vector<std::unique_ptr<EventNodeBox>> boxes;
	std::unique_ptr<Camera> camera;

	TriggerBox* entry;

	bool is_plugging = false;
	EventNodeBox* plugging_out_box = nullptr;
	int plug_out = -1;
	EventNodeBox* plugging_in_box = nullptr;

	EventNodeBox* selected_box = nullptr;
	bool is_resizing_ = false;
	bool is_moving_ = false;

	EventNodeBox* get_box_from_uuid(const Guid& i) {
		for (auto& n : boxes) {
			if (n->guid() == i)
				return n.get();
		}
		return nullptr;
	}
};