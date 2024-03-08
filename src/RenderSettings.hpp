/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#pragma warning(push, 0)
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#pragma warning(pop)

class Dungeon;

struct RenderSettings final {
	SDL_Window*   window{};
	SDL_Renderer* renderer{};
	SDL_Texture*  texture{};

	RenderSettings();

	bool createWindow();
	bool createRenderer();
	bool createTexture();

	void errorMessage(const char*);

	const s32 window_W;
	const s32 window_H;

	s32  pitch;
	u32* pixels{};
	u64  frames{};

	void lockTexture();
	void unlockTexture();
	void present();
	void quit();

	void flushDisplay(Dungeon&, const int, const int);
};
