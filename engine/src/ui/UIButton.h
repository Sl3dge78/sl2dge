#pragma once

#include <functional>

#include "ECS/Component.h"

namespace sl2dge {

class UIButton : public Component {
	COMPONENT_DFLT_BEHAVIOUR(UIButton)

public:
	UIButton(){};
	UIButton(std::function<void()> on_click);

	virtual void handle_events(Game *game, SDL_Event const &e) override;

protected:
	int w = 16;
	int h = 16;

private:
	std::function<void()> on_click_ = nullptr;
};

} // namespace sl2dge
