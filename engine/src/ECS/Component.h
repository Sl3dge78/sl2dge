#pragma once

#include <fstream>
#include <string>

#include <SDL/SDL.h>

#include "addons/pugixml.hpp"

namespace sl2dge {
class Entity;

class Component {
	friend class Entity;

public:
	Component() = default;
	virtual ~Component() = 0;

protected:
	static void open_xml_doc(pugi::xml_document *doc, const std::string &map_path) {
		pugi::xml_parse_result result = doc->load_file(map_path.c_str());
		if (!result) {
			SDL_Log("Unable to read xml %s : %s", map_path.c_str(), result.description());
			if (result.status == pugi::xml_parse_status::status_bad_attribute) {
				std::ifstream file;
				file.open(map_path);
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
	Entity *entity() { return entity_; }

private:
	Entity *entity_;
};
inline Component::~Component() = default;

// Used by entities to refer to components with an unique id
struct ComponentID {
public:
	template <class T>
	static const int Get() {
		static int id = count++;
		return id;
	}

private:
	static int count;
};
} // namespace sl2dge
