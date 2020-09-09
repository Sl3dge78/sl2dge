#pragma once

#include "ECS/ECS.h"
#include "UIPanel.h"

namespace sl2dge {

class UIContextMenu : public UIPanel {
	COMPONENT_DFLT_BEHAVIOUR(UIContextMenu)

public:
	UIContextMenu() = default;
	UIContextMenu(const int w, const int h, const SDL_Color &color);

	virtual void load(const pugi::xml_node &node) override;
	virtual void save(pugi::xml_node &node) override;

	virtual void handle_events(Game *game, SDL_Event const &e) override;
};

} // namespace sl2dge
