/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"
#include "Dungeon.hpp"

RenderSettings::RenderSettings()
	: window_W(800)
	, window_H(800)
	, pitch(window_W * 4)
{}

bool RenderSettings::createWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"Dungeon Explorer v.1",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_W, window_H,
		SDL_WINDOW_INPUT_FOCUS |
		SDL_WINDOW_ALLOW_HIGHDPI
	);
	if (window) return true;
	errorMessage("Window init error");
	return false;
}
bool RenderSettings::createRenderer() {
	renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC
	);
	if (renderer) return true;
	errorMessage("Renderer init error");
	return false;
}
bool RenderSettings::createTexture() {
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_BGRA32,
		SDL_TEXTUREACCESS_STREAMING,
		DUNGEON_W, DUNGEON_H
	);

	if (texture) return true;

	errorMessage("Texture init error");
	return false;
}

void RenderSettings::errorMessage(const char* error) {
	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		error, SDL_GetError(),
		window
	);
}

void RenderSettings::lockTexture() {
	void* pixel_ptr{ pixels };
	SDL_LockTexture(texture, nullptr, as<void**>(&pixel_ptr), &pitch);
	pixels = as<u32*>(pixel_ptr);
}
void RenderSettings::unlockTexture() {
	SDL_UnlockTexture(texture);
}

void RenderSettings::renderPresent() {
	++frames;
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}
void RenderSettings::quit() {
	if (texture)  SDL_DestroyTexture(texture);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window)   SDL_DestroyWindow(window);
	SDL_Quit();
}

void RenderSettings::flushDisplay(Dungeon& dungeon, const int curY, const int curX) {
	static const auto additive{ [](const u32 color, const double pc) -> u32 {
		const auto r{ as<u32>((color >> 16 & 0xFFu) * pc) };
		const auto g{ as<u32>((color >>  8 & 0xFFu) * pc) };
		const auto b{ as<u32>((color       & 0xFFu) * pc) };

		Well512 rng;
		const auto rnd{ (rng() % 7) };

		const auto result{
			0xFF000000u
			| std::min(r + rnd, 0xFFu) << 16
			| std::min(g + rnd, 0xFFu) <<  8
			| std::min(b + rnd, 0xFFu)
		};
		return result;
	} };

	enum Colors : u32 {
		C_DARK  = 0xFF000000,
		C_PATH  = 0xFF1C1C1C,
		C_WALL  = 0xFF707070,
		C_HERO  = 0xFF30A0D0,
		C_ENEMY = 0xFFD01810,
		C_CHEST = 0xFFF0D818,
		C_HPOT  = 0xFF98E080,
	};

	lockTexture();

	for (auto Y{ 0 }; Y < DUNGEON_H; ++Y) {
		for (auto X{ 0 }; X < DUNGEON_W; ++X) {

			const auto dY{ std::abs(curY - Y) };
			const auto dX{ std::abs(curX - X) };
			const auto dist{ std::sqrt(dX * dX + dY * dY) };
			const auto radius{ 5.1 + std::array{
				0.0, 0.15, 0.30, 0.45, 0.60, 0.45, 0.30, 0.15
			}[(frames / 10) & 7] };

			if (std::round(dist) >= radius) {
				*pixels++ = additive(Colors::C_DARK, 1.0);
				continue;
			}

			const auto near{ std::max(1.0 - (dist / radius), 0.0) };
			const auto sine{ std::sin(near * M_PI / 2.0) };

			switch (dungeon.map[Y][X]) {
				case Dungeon::TileID::PATH:
					*pixels++ = additive(Colors::C_PATH, sine);
					break;
				case Dungeon::TileID::WALL:
					*pixels++ = additive(Colors::C_WALL, sine);
					break;
				case Dungeon::TileID::HERO:
					*pixels++ = additive(Colors::C_HERO, sine);
					break;
				case Dungeon::TileID::ENEMY:
				case Dungeon::TileID::BOSS:
					*pixels++ = additive(Colors::C_ENEMY, sine);
					break;
				case Dungeon::TileID::CHEST:
					*pixels++ = additive(Colors::C_CHEST, sine);
					break;
				case Dungeon::TileID::HPOT:
					*pixels++ = additive(Colors::C_HPOT, sine);
					break;
			}
		}
	}

	pixels -= DUNGEON_SIZE;
	unlockTexture();
	renderPresent();
}

