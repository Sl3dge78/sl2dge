#pragma once

#include <stdexcept>

#include <SDL/SDL.h>

#include "Camera.h"
#include "Rect.h"

// Simple Light 2D Game Engine

namespace sl2dge {

class Camera;

enum class Direction { Up,
	Down,
	Left,
	Right };

void RenderCopy(SDL_Renderer &renderer, SDL_Texture &texture, const Rect *src, const Rect *dst);

// Wrapper for SDL_RenderCopy but to account for world position relative to camera. Won't draw if it's out of the viewport.
void RenderCopyWorld(SDL_Renderer &renderer, SDL_Texture &texture, const Camera &camera, const Rect *src, const Rect *dst);
} // namespace sl2dge
