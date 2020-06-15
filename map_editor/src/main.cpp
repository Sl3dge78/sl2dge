#include <iostream>
#include <fstream>
#include <memory>

#include "sl2dge.h"
#include "MapEditor.h"
#include "EventEditor.h"

using namespace sl2dge;


int main(int argc, char* argv[]) {
	//_crtBreakAlloc = 285;

	//NewMap("resources/levels/map2.map", "images\\roguelikeSheet_transparent.png", 16, 57, 32, 100, 100);

	Game* app = new Game("Super Map Editor 3000", 1280, 720);

	app->change_state(std::make_unique<Editor>("resources/levels/maptest.map"));
	//app->change_state(std::make_unique<EventEditor>("resources/levels/maptest.map", SDL_Point{ 10, 0 }));

	app->loop();
	delete app;
	_CrtDumpMemoryLeaks();
	return 1;
}

