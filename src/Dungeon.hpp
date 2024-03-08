/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"

constexpr std::size_t DUNGEON_W{ 40 };
constexpr std::size_t DUNGEON_H{ 40 };
constexpr std::size_t DUNGEON_SIZE{ DUNGEON_W * DUNGEON_H };

enum TileID : u32 {
	PATH  = ' ',
	WALL  = '#',
	HERO  = 'P',
	BOSS  = 'B',
	ENEMY = 'E',
	CHEST = 'C',
	HPOT  = 'H',
};

/*



*/



class Item;
class Enemy;
class Player;

#include "Item.hpp"
#include "Monster.hpp"
#include "Player.hpp"

class Dungeon final {
public:
	arr2D<u8, DUNGEON_W, DUNGEON_H> map{};
	objVector<Enemy> monsters{};
	objVector<Item>  treasure{};
	objVector<Item>  healpots{};
	objVector<Item>  lootpool{};
	Player hero;
	Well512 rng;

	enum class Phase {
		ROLL    = 0,
		BATTLE  = 1,
		STANDBY = 2,
		END     = 3,
	};

	bool inCombat{ false };
	Phase currentPhase{};

	Dungeon();
	void defineTreasurePool();
	void updatePlayerPos(const int Y, const int X);
	bool readMapFile();
};
