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

#include "Item.hpp"
#include "Monster.hpp"
#include "Player.hpp"

namespace Map {
	enum class TileID : u8 {
		PATH  = ' ',
		WALL  = '#',
		HERO  = 'P',
		BOSS  = 'B',
		ENEMY = 'E',
		CHEST = 'C',
		HPOT  = 'H',
	};
}

class DungeonMap {
	std::wstring filepath{};

public:
	DungeonMap(std::wstring file) : filepath(file) {}
	bool verifyMap();
};



class Dungeon final : protected BasicRenderer {
public:
	arr2D<Map::TileID, DUNGEON_W, DUNGEON_H> map{};
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
	bool readMap();
	void flushDisplay();
};
