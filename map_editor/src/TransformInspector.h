#pragma once

#include "sl2dge.h"
using namespace sl2dge;

class TransformInspector : public GameState {
public:
	TransformInspector(Transform *transform);
	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, const SDL_Event &e) override;
	virtual void input(Game *game) override;
	virtual void update(Game *game) override;
	virtual void draw(Game *game) override;
	void quit(Game *game);

private:
	Transform *transform_ = nullptr;
	World world;
};
