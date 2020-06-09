#pragma once

#include <vector> 
#include <iostream>
#include <fstream>

#include "sl2dge.h"

#include "EventNodeBox.h"

using namespace sl2dge;

class EventEditor : public GameState {
public:

	EventEditor(const std::string& map_path, const SDL_Point& map_pos) : map_path_(map_path), map_pos_(map_pos) {}

	// Inherited via GameState
	virtual void start(Game* game) override;
	virtual void handle_events(Game* game, const SDL_Event& e) override;
	virtual void input(Game* game) override;
	virtual void update(Game* game) override;
	virtual void draw(Game* game) override;
	virtual void on_state_resume(Game* game) override;
	virtual void on_state_pause(Game* game) override;

private:
	std::unique_ptr<std::vector<std::unique_ptr<EventNodeBox>>> boxes;
	//TriggerBox* trigger_ = nullptr;
	std::unique_ptr<Camera> camera;

	SDL_Point map_pos_;
	std::string map_path_;

	bool is_plugging = false;
	EventNodeBox* plugging_out_box = nullptr;
	int plug_out = -1;
	EventNodeBox* plugging_in_box = nullptr;

	EventNodeBox* selected_box = nullptr;
	bool is_resizing_ = false;
	bool is_moving_ = false;

	EventNodeBox* get_box_from_uuid(const Guid& i) {
		for (auto& n : *boxes) {
			if (n->guid() == i)
				return n.get();
		}
		return nullptr;
	}

	void open_xml_doc(pugi::xml_document* doc, std::string& map_path) {
		pugi::xml_parse_result result = doc->load_file(map_path_.c_str());
		if (!result) {
			SDL_Log("Unable to read xml %s : %s", map_path_.c_str(), result.description());
			if (result.status == pugi::xml_parse_status::status_bad_attribute) {
				std::ifstream file;
				file.open(map_path_);
				file.seekg(result.offset);

				std::string s;
				s.resize(20);
				file.read(&s[0], 20);
				SDL_Log("Error at [...%s]", s.c_str());
			}
			doc = new pugi::xml_document();
		}

		SDL_Log("%s successfully loaded", map_path.c_str());
	}


	bool get_event_chain_node(pugi::xml_document& doc, std::string& path, pugi::xml_node& node) {
		using namespace pugi;

		auto events_node = doc.child("Events");
		if (!events_node) {
			events_node = doc.append_child("Events");
		}

		bool found = false;
		if (!events_node.child("Event_Chain")) { // No event, in the current doc, create it and goo
			node = events_node.append_child("Event_Chain");
			return false;
		} else { // If there are already events in the document iterate through them to find the right one.
			for (auto event : events_node.children("Event_Chain")) {
				if (event.attribute("x_pos").as_int() == map_pos_.x && event.attribute("y_pos").as_int() == map_pos_.y) {
					node = event;
					return true;
				}
			}
			// No node found
			node = events_node.append_child("Event_Chain");
			return false;
			
		}
	}

	void save();
};