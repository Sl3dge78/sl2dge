#pragma once

#include "ECS/Component.h"

namespace sl2dge {

class UIButton : public Component {
	COMPONENT_DFLT_BEHAVIOUR(UIButton)

	typedef void (*EventCallback)(Game *game, UIButton *button);

public:
	UIButton();

	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;
	virtual void handle_events(Game *game, SDL_Event const &e) override;

	static Uint32 ON_CLICK;

private:
	int w = 10;
	int h = 10;

	SDL_Event event_;

	virtual void start(Game *game) override;
	virtual void update(Game *game) override;
	virtual void draw(Game *game) override;
};

} // namespace sl2dge
