/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#pragma warning(push)
#pragma warning(disable : 26819)
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#pragma warning(pop)

struct BasicRenderer {
	SDL_Window*   window{};
	SDL_Renderer* renderer{};
	SDL_Texture*  texture{};

	BasicRenderer();

	bool createWindow();
	bool createRenderer();
	bool createTexture(const s32, const s32);

	void errorMessage(const char*);

	const s32 window_W;
	const s32 window_H;

	s32  pitch{};
	u32* pixels{};
	u64  frames{};

	void lockTexture();
	void unlockTexture();
	void renderPresent();
	void quitWindow();
	void quitRenderer();
	void quitTexture();
	void quitSDL();
};
