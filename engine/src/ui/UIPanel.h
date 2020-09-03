#pragma once

#include "ECS/ECS.h"
#include "Game.h"

namespace sl2dge {

class UIPanel : public Component {
	COMPONENT_DFLT_BEHAVIOUR(UIPanel)
public:
	UIPanel() {}
	UIPanel(const int w, const int h, const SDL_Color &color);

	// Inherited via Component
	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;

	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, SDL_Event const &e) override;
	virtual void update(Game *game) override;
	virtual void draw(Game *game) override;

private:
	int w_ = 10;
	int h_ = 10;
	SDL_Color color_ = { 255,
		255,
		255,
		255 };
};

} // namespace sl2dge
