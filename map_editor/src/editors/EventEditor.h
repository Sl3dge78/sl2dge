//#pragma once
//
//#include <fstream>
//#include <iostream>
//#include <vector>
//
//#include "UI.h"
//#include "sl2dge.h"
//
//using namespace sl2dge;
//
//class EventEditor : public GameState {
//public:
//	EventEditor(Scene *scene, EventChain *event_chain);
//
//	// Inherited via GameState
//	virtual void start(Game *game) override;
//	virtual void handle_events(Game *game, const SDL_Event &e) override;
//	virtual void input(Game *game) override;
//	virtual void update(Game *game) override;
//	virtual void draw(Game *game) override;
//	virtual void on_state_resume(Game *game) override;
//	virtual void on_state_pause(Game *game) override;
//	virtual void on_state_exit(Game *game) override;
//
//private:
//	std::unique_ptr<Camera> camera;
//	Scene *scene_;
//	EventChain *event_chain_;
//
//	std::unique_ptr<ToggleBox> interactable_ = nullptr;
//	std::unique_ptr<ToggleBox> is_in_place_ = nullptr; //  you need to stand on the item to trigger the event
//	std::unique_ptr<ToggleBox> activate_once_ = nullptr; // If true, won't activate ever again
//
//	bool is_plugging = false;
//	GameEvent *plugging_out_box = nullptr;
//	int plug_out = -1;
//	GameEvent *plugging_in_box = nullptr;
//
//	GameEvent *selected_box = nullptr;
//	bool is_resizing_ = false;
//	bool is_moving_ = false;
//
//	void remove_all_connections_to(Guid id); // Will search all boxes and remove the connection that connects to id
//	void save();
//};
