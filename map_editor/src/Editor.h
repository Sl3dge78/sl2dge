#pragma once

#include "sl2dge.h"

namespace sl2dge {
class Editor : public GameState {
public:
	Editor(const std::string &map_path);
	~Editor();

	// Inherited via GameState
	virtual void start(Game *game) override;
	virtual void handle_events(Game *game, const SDL_Event &e) override;
	virtual void input(Game *game) override;
	virtual void update(Game *game) override;
	virtual void draw(Game *game) override;

private:
	std::string map_path_;
	std::unique_ptr<World> scene_;
	std::unique_ptr<World> editor_;
	Camera *map_camera_ = nullptr;
	int current_layer = 0;

	Entity *entity_panel = nullptr;
	UIButton *add_entity_but = nullptr;

	void create_ui(Game *game);

	void on_add_entity_click(Game *game);
};
} // namespace sl2dge
