#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <fstream>
#include <iostream>
#include <memory>

#include "sl2dge.h"

#include "Editor.h"

//#include "EventEditor.h"

using namespace sl2dge;

int main(int argc, char *argv[]) {
	//_crtBreakAlloc = 285;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//NewMap("resources/levels/map2.map", "images\\roguelikeSheet_transparent.png", 16, 57, 32, 100, 100);

	Game *app = new Game("Super Map Editor 3000", 1280, 720, Game::Platform::Editor);

	app->change_state(std::make_unique<Editor>("resources/test.map"));
	//app->change_state(std::make_unique<EventEditor>("resources/levels/maptest.map", SDL_Point{ 10, 0 }));

	app->loop();
	delete app;
	return 1;
}
