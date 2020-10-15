#pragma once

#include "sl2dge.h"

namespace sl2dge {

class UINumberField : public Component {
public:
	virtual void draw(Game *game) override;
	virtual void handle_events(Game *game, SDL_Event const &e) override;

protected:
	UINumberField(FC_Font *font) {
		font_ = font;
	}

private:
	virtual void on_text_edit_end(const std::string entered_text) = 0;
	virtual std::string get_field_string() = 0;

	FC_Font *font_ = nullptr;

	bool is_editing_text = false;
	std::string current_text_ = "";
	int width_ = 80, height_ = 16;

	SDL_Rect get_text_box() { return SDL_Rect{ int(transform()->position().x), int(transform()->position().y), width_, height_ }; }
};

class UIFloatField : public UINumberField {
	COMPONENT_DFLT_BEHAVIOUR(UIFloatField)

public:
	UIFloatField(float *field, FC_Font *font) :
			UINumberField(font), field_(field) {
	}

private:
	float *field_;

	// Inherited via UINumberField
	virtual void on_text_edit_end(const std::string entered_text) override;
	virtual std::string get_field_string() override;
};

class UIIntField : public UINumberField {
	COMPONENT_DFLT_BEHAVIOUR(UIIntField)

public:
	UIIntField(int *field, FC_Font *font) :
			UINumberField(font), field_(field) {
	}

private:
	int *field_;

	// Inherited via UINumberField
	virtual void on_text_edit_end(const std::string entered_text) override;
	virtual std::string get_field_string() override;
};

} // namespace sl2dge
