#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <memory>

#include <sl2dge.h>

#include "MainState.h"
#include "PostOffice.h"

using namespace sl2dge;

int main(int argc, char* argv[]) {
	//_crtBreakAlloc = 3724;
	Game* app = new Game("", 320, 288);
	app->change_state(std::make_unique<MainState>());
	app->loop();
	delete app;
	_CrtDumpMemoryLeaks();
	return 1;
}