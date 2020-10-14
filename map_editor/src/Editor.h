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
	virtual void cleanup(Game *game) override;

	virtual void on_state_resume(Game *game);
	virtual void on_state_pause(Game *game);
	virtual void on_state_exit(Game *game);

	void on_add_entity_click();
	void on_delete_entity_click(Entity *entity);
	void on_delete_component_click(Component *component);
	void on_add_component_click(Game *game, Entity *e, int y);
	void on_add_component_to_click(Entity *e, int comp_id);

	void on_component_click(Game *game, Component *component);

private:
	std::string map_path_;
	std::unique_ptr<World> scene_;
	std::unique_ptr<World> editor_;
	Camera *map_camera_ = nullptr;
	int current_layer = 0;

	Entity *ui_root = nullptr;
	Entity *entity_panel = nullptr;
	UIButton *add_entity_but = nullptr;
	bool entity_list_dirty = false;

	bool is_focused = true;

	void create_ui(Game *game);
	void update_entity_list(Game *game);
};
} // namespace sl2dge
