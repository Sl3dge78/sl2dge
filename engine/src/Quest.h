#pragma once
#include <string>
#include <vector>

class Quest {
public:
	Quest(const int id, const std::string &name) :
			id(id), name(name) {
		status = 0;
	}
	~Quest(){};

	int id;
	std::string name;
	int status = 0;
};
