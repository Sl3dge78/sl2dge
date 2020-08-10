#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "ECS/ECS.h"
#include "ECS/ECS_DB.h"
#include "Game.h"
#include "addons/pugixml.hpp"

namespace sl2dge {

class Scene : public World {
public:
	Scene(const std::string &path);
	~Scene(){};

	void load_from_xml(const std::string &path);
	void save();

private:
	std::string path_;
};

} // namespace sl2dge
