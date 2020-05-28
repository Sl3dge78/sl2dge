#pragma once

#include <vector> 
#include <concepts>

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
	std::vector<EventNodeBox> boxes;
	std::unique_ptr<Camera> camera;

	bool is_plugging = false;
	int plugging_out_id = -1;
	int plug_out = -1;
	int plugging_in_id = -1;

	EventNodeBox* get_box_from_id(int i) {
		for (auto& n : boxes) {
			if (n.id() == i)
				return &n;
		}
		return nullptr;
	}
};