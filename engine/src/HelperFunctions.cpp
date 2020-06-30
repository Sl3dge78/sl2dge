#include "HelperFunctions.h"

#include "scene/Camera.h"

void sl2dge::RenderCopy(SDL_Renderer &renderer, SDL_Texture &texture, const Rect *src, const Rect *dst) {
	SDL_ClearError();
	SDL_Rect *src_sdl;
	if (src != NULL)
		src_sdl = new SDL_Rect{ src->x, src->y, src->w, src->h };
	else
		src_sdl = NULL;
	SDL_Rect *dst_sdl;
	if (dst != NULL)
		dst_sdl = new SDL_Rect{ dst->x, dst->y, dst->w, dst->h };
	else
		dst_sdl = NULL;
	SDL_RenderCopy(&renderer, &texture, src_sdl, dst_sdl);
	auto dd = SDL_GetError();

	delete src_sdl;
	delete dst_sdl;
}

// Wrapper for SDL_RenderCopy but to account for world position relative to camera. Won't draw if it's out of the viewport.
void sl2dge::RenderCopyWorld(SDL_Renderer &renderer, SDL_Texture &texture, const Camera &camera, const Rect *src, const Rect *dst) {
	if (dst == NULL) {
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "No destination specified for World Rendering");
		return;
	}
	//if (!camera.viewport().ContainsPartially(*dst))
	//	return;

	Rect screen_dest = camera.world_to_screen_transform(*dst);
	RenderCopy(renderer, texture, src, &screen_dest);
}
