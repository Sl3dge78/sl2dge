#include <iostream>
#include <fstream>
#include <memory>

#include "sl2dge.h"
#include "MapEditor.h"
#include "EventEditor.h"

using namespace sl2dge;

void NewMap(const std::string& path, const std::string& atlas_path, const int tile_size, const int tiles_x, const int tiles_y, const int width, const int height) {
	using namespace std;

	ofstream file;
	file.open(path);

	if (!file.is_open()) {
		cout << "Error opening file: " << path << endl;
		return;
	}

	file << atlas_path << " ";
	file << tile_size << " ";
	file << tiles_x << " ";
	file << tiles_y << " ";
	file << width << " ";
	file << height << " ";
	file << endl;
	for (int i = 0; i < width * height; ++i) {
		file << 0 << " ";
	}
	file << endl;

	for (int i = 0; i < width * height; ++i) {
		file << -1 << " ";
	}
	file << endl;

	for (int i = 0; i < width * height; ++i) {
		file << -1 << " ";
	}
	file << endl;

	for (int i = 0; i < width * height; ++i) {
		file << -1 << " ";
	}

	file.close();
}

int main(int argc, char* argv[]) {
	//_crtBreakAlloc = 285;

	//NewMap("resources/levels/map2.map", "images\\roguelikeSheet_transparent.png", 16, 57, 32, 100, 100);

	Game* app = new Game("Super Map Editor 3000", 1280, 720);

	//app->change_state(std::make_unique<Editor>("resources/levels/map2.map"));
	app->change_state(std::make_unique<EventEditor>());

	app->loop();
	delete app;
	_CrtDumpMemoryLeaks();
	return 1;
}

