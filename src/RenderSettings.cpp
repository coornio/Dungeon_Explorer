/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"

BasicRenderer::BasicRenderer()
	: window_W(800) // placeholder
	, window_H(800) // placeholder
{}

bool BasicRenderer::createWindow() {
	quitWindow();
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
bool BasicRenderer::createRenderer() {
	quitRenderer();
	renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC
	);
	if (renderer) return true;
	errorMessage("Renderer init error");
	return false;
}
bool BasicRenderer::createTexture(const s32 WIDTH, const s32 HEIGHT) {
	quitTexture();
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_BGRA32,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH, HEIGHT
	);
	pitch = WIDTH * 4;
	if (texture) return true;

	errorMessage("Texture init error");
	return false;
}

void BasicRenderer::errorMessage(const char* error) {
	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		error, SDL_GetError(),
		window
	);
}

void BasicRenderer::lockTexture() {
	void* pixel_ptr{ pixels };
	SDL_LockTexture(texture, nullptr, as<void**>(&pixel_ptr), &pitch);
	pixels = as<u32*>(pixel_ptr);
}
void BasicRenderer::unlockTexture() {
	SDL_UnlockTexture(texture);
}

void BasicRenderer::renderPresent() {
	++frames;
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}
void BasicRenderer::quitTexture() {
	if (texture)  SDL_DestroyTexture(texture);
}
void BasicRenderer::quitRenderer() {
	if (renderer) SDL_DestroyRenderer(renderer);
}
void BasicRenderer::quitWindow() {
	if (window)   SDL_DestroyWindow(window);
}
void BasicRenderer::quitSDL() {
	quitTexture();
	quitRenderer();
	quitWindow();
	SDL_Quit();
}
