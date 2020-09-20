#include "Editor.h"

namespace sl2dge {

Editor::Editor(const std::string &map_path) {
	map_path_ = map_path;
	scene_ = std::make_unique<World>(map_path_);

	editor_ = std::make_unique<World>();

	auto camera = editor_->create_entity();
	camera->add_component<Transform>(0.0f, 0.0f);
	map_camera_ = camera->add_component<Camera>(1280, 720);
	Camera::main_camera = map_camera_;
}

Editor::~Editor() {
}

void Editor::start(Game *game) {
	create_ui(game);

	scene_->start(game);
	editor_->start(game);
}

void Editor::handle_events(Game *game, const SDL_Event &e) {
	editor_->handle_events(game, e);

	if (e.type == SDL_MOUSEWHEEL) {
		if (e.wheel.y < 0) {
			float zoom = map_camera_->zoom_;
			if (zoom <= 1) {
				zoom /= 2.0;
			} else {
				--zoom;
			}
			map_camera_->zoom_ = zoom;

		} else if (e.wheel.y > 0) {
			float zoom = map_camera_->zoom_;
			if (zoom < 1) {
				zoom *= 2.0;
			} else {
				++zoom;
			}
			map_camera_->zoom_ = zoom;
		}
	}
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.scancode) {
			case SDL_SCANCODE_1:
				current_layer = 0;
				break;
			case SDL_SCANCODE_2:
				current_layer = 1;
				break;
			case SDL_SCANCODE_3:
				current_layer = 2;
				break;
			case SDL_SCANCODE_4:
				current_layer = 3;
				break;
			case SDL_SCANCODE_F5:
				scene_->save(map_path_);
		}
	}
}

void Editor::input(Game *game) {
	const auto state = SDL_GetKeyboardState(NULL);
	Vector2f movement = { 0, 0 };
	if (state[SDL_SCANCODE_W])
		movement.y += -1;
	if (state[SDL_SCANCODE_S])
		movement.y += 1;
	if (state[SDL_SCANCODE_D])
		movement.x += 1;
	if (state[SDL_SCANCODE_A])
		movement.x += -1;
	movement.normalize();
	movement *= 700 / map_camera_->zoom_ * float(game->delta_time()) / 1000.0f;
	map_camera_->entity()->transform()->translate(movement);
}

void Editor::update(Game *game) {
	editor_->update(game);
}

void Editor::draw(Game *game) {
	for (int i = 0; i <= current_layer; ++i) {
		SDL_SetRenderDrawBlendMode(game->renderer(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 100);
		SDL_RenderFillRect(game->renderer(), NULL);
		scene_->draw_layer(game, i);
	}
	editor_->draw(game);
}

void Editor::cleanup(Game *game) {
	scene_->cleanup(game);
	editor_->cleanup(game);

	if (entity_list_dirty) {
		update_entity_list(game);
		entity_list_dirty = false;
	}
}

void Editor::create_ui(Game *game) {
	SDL_Color d_gray = SDL_Color{ 25, 25, 25, 255 };
	ui_root = editor_->create_entity(0, 0);

	entity_list_dirty = true;

	auto bottom_panel = editor_->create_entity(0, 700, ui_root);
	bottom_panel->add_component<UIPanel>(1280, 20, d_gray);

	auto input_tip = editor_->create_entity(0, 0, bottom_panel)->add_component<UIText>("W, A, S, D : Move cam ; F5 : Save scene", game->white_font());
}

void Editor::update_entity_list(Game *game) {
	if (entity_panel != nullptr)
		editor_->delete_entity(entity_panel);

	entity_panel = editor_->create_entity(0, 0, ui_root);
	entity_panel->add_component<UIPanel>(200, 704, SDL_Color{ 25, 25, 25, 255 });

	int y = 1;
	for (auto &e : *scene_->all_entities()) {
		auto entity_ptr = e.get();

		auto entity_text = editor_->create_entity(16.0f, y * 20.0f, entity_panel);
		entity_text->add_component<UIText>("Entity", game->white_font());

		auto delete_entity = editor_->create_entity(-16, 0, entity_text);
		delete_entity->add_component<UIText>("-", game->white_font());
		delete_entity->add_component<UIButton>([entity_ptr, this]() { this->on_delete_entity_click(entity_ptr); });

		auto add_component = editor_->create_entity(200 - 32, 0, entity_text);
		add_component->add_component<UIText>("+", game->white_font());
		add_component->add_component<UIButton>([this, game, entity_ptr, y]() { this->on_add_component_click(game, entity_ptr, y * 20); });

		y++;
		int y2 = 0;
		for (auto comp : e->all_components()) {
			auto comp_text = editor_->create_entity(32.0f, 20 + y2 * 20.0f, entity_text);
			comp_text->add_component<UIText>(comp->type_name(), game->white_font());

			y2++;
			y++;

			if (comp->type_name() == "Transform")
				continue;

			auto delete_component = editor_->create_entity(-16, 0, comp_text);
			delete_component->add_component<UIText>("-", game->white_font());
			delete_component->add_component<UIButton>([comp, this]() { this->on_delete_component_click(comp); });
		}
	}

	auto add_entity = editor_->create_entity(200 - 16, 0, entity_panel);
	add_entity_but = add_entity->add_component<UIButton>([this]() { this->on_add_entity_click(); });
	add_entity->add_component<UIText>("+", game->white_font());
}

void Editor::on_add_entity_click() {
	scene_->create_entity(0, 0);
	entity_list_dirty = true;
	SDL_Log("Create entity!");
}
void Editor::on_delete_entity_click(Entity *entity) {
	scene_->delete_entity(entity);
	entity_list_dirty = true;
	SDL_Log("Delete entity!");
}

void Editor::on_delete_component_click(Component *component) {
	component->entity()->remove_component(component);
	entity_list_dirty = true;
	SDL_Log("Delete component!");
}

void Editor::on_add_component_click(Game *game, Entity *e, int y) {
	auto context_menu = editor_->create_entity(200, y);
	int amount = Component::component_amount();
	context_menu->add_component<UIContextMenu>(100, amount * 16, SDL_Color{ 25, 25, 25, 255 });
	for (int i = 0; i < amount; i++) {
		auto component_label = editor_->create_entity(0, i * 16, context_menu);
		component_label->add_component<UIButton>([e, i, this]() { this->on_add_component_to_click(e, i); }, 100, 16);
		component_label->add_component<UIText>(Component::get_type_name(i), game->white_font());
	}
}

void Editor::on_add_component_to_click(Entity *e, int comp_id) {
	if (e->has_component(comp_id))
		return;

	e->add_component_from_id(comp_id);
	entity_list_dirty = true;
	SDL_Log("Added Component %s", Component::get_type_name(comp_id).c_str());
}

} // namespace sl2dge
