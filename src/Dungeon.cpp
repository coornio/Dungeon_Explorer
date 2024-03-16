/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

//#include "Includes.hpp"
#include "Dungeon.hpp"

Dungeon::Dungeon() {
	defineTreasurePool();
}

void Dungeon::defineTreasurePool() {
	lootpool.insertEntry(
		Weapon(L"Dagger"sv, 4,
		L"Standard-issue dagger, the blade could use some sharpening."sv)
	);
	lootpool.insertEntry(
		Weapon(L"Short-sword"sv, 6,
		L"Longer reach than the dagger, light and in good condition."sv)
	);
	lootpool.insertEntry(
		Weapon(L"Long-sword"sv, 8,
		L"A considerable upgrade, the weight is perfectly balanced."sv)
	);
	lootpool.insertEntry(
		Weapon(L"Battle-axe"sv, 12,
		L"The pole is engraved and the axe-head is ridiculously sharp."sv)
	);
	lootpool.insertEntry(
		Armour(L"Leather Armour"sv, 2,
		L"Scuffed and worn-out, but better than wearing nothing at all."sv)
	);
	lootpool.insertEntry(
		Armour(L"Chainmaille"sv, 4,
		L"A bit on the noisy side but surprisingly well maintained."sv)
	);
	lootpool.insertEntry(
		Armour(L"Breastplate"sv, 6,
		L"Solid plate of steel but it is missing the rest of the set."sv)
	);
	lootpool.insertEntry(
		Armour(L"Full Plate Armour"sv, 8,
		L"A complete suit of armour made of durable damascus steel."sv)
	);
}

void Dungeon::updatePlayerPos(const int Y, const int X) {
	int curY = hero.getPosY();
	int curX = hero.getPosX();

	int newY = curY + Y;
	int newX = curX + X;

	switch (map[newY][newX]) {
		case Map::TileID::WALL:
			return;
		default:
			map[curY][curX] = Map::TileID::PATH;
			map[newY][newX] = Map::TileID::HERO;
			hero.setPosition(newY, newX);
	}

	for (auto idx{ 0 }; idx < treasure.size(); ++idx) {
		auto& elem{ treasure[idx] };
		if (newY != elem.getPosY()) continue;
		if (newX != elem.getPosX()) continue;

		elem.encounterMessage();
		treasure.deleteEntry(idx);
		const auto  random{ rng() % lootpool.size() };
		const auto& pickedItem{ lootpool[random] };
		const bool  canEquip{ hero.canEquip(pickedItem) };

		pickedItem.acquiredMessage(canEquip);
		if (canEquip) hero.takeItem(random, lootpool, true);
		return;
	}

	for (auto idx{ 0 }; idx < healpots.size(); ++idx) {
		auto& elem{ healpots[idx] };
		if (newY != elem.getPosY()) continue;
		if (newX != elem.getPosX()) continue;

		elem.encounterMessage();
		elem.acquiredMessage(true);
		hero.takeItem(idx, healpots, false);
		return;
	}

	for (auto idx{ 0 }; idx < monsters.size(); ++idx) {
		auto& elem{ monsters[idx] };
		if (newY != elem.getPosY()) continue;
		if (newX != elem.getPosX()) continue;

		elem.encounterMessage();
		inCombat = true;
		// initiate combat here
		return;
	}
}

bool DungeonMap::verifyMap() {
	namespace fs = std::filesystem;

	const fs::path fspath{ "map.txt" };
	if (!fs::exists(fspath) || !fs::is_regular_file(fspath)) {
		std::cerr << "Unable to locate file: " << fspath.string() << std::endl;
		return false;
	}

	std::error_code ec;
	const auto ifs_length{ fs::file_size(fspath, ec) };
	if (ec) {
		std::cerr << "Unable to access file: " << fspath.string() << std::endl;
		return false;
	}
	return true;
}

bool Dungeon::readMap() {
	namespace fs = std::filesystem;

	const fs::path fspath{ "map.txt" };
	if (!fs::exists(fspath) || !fs::is_regular_file(fspath)) {
		std::cerr << "Unable to locate file: " << fspath.string() << std::endl;
		return false;
	}

	std::error_code ec;
	const auto ifs_length{ fs::file_size(fspath, ec) };
	if (ec) {
		std::cerr << "Unable to access file: " << fspath.string() << std::endl;
		return false;
	}

	std::ifstream ifs(fspath);
	for (auto Y{ 0 }; Y < DUNGEON_H; ++Y) {
		for (auto X{ 0 }; X < DUNGEON_W; ++X) {

			char ch{};
			ifs.get(ch);
			if (ch == '\n') {
				--X;
				continue;
			}

			auto tile{ as<Map::TileID>(ch) };
			map[Y][X] = tile;

			switch (tile) {
				case Map::TileID::HERO:
					hero.setPosition(Y, X);
					break;
				case Map::TileID::ENEMY:
					monsters.createEntry<Monster>(Y, X, false);
					break;
				case Map::TileID::BOSS:
					monsters.createEntry<Monster>(Y, X, true);
					break;
				case Map::TileID::CHEST:
					treasure.createEntry<Treasure>(Y, X);
					break;
				case Map::TileID::HPOT:
					healpots.createEntry<Potion>(Y, X, (rng() % 31) + 10);
					break;
			}
		}
	}
	return true;
}

void Dungeon::flushDisplay() {
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

	const auto curY{ hero.getPosY() };
	const auto curX{ hero.getPosX() };

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

			switch (map[Y][X]) {
				case TileID::PATH:
					*pixels++ = additive(Colors::C_PATH, sine);
					break;
				case TileID::WALL:
					*pixels++ = additive(Colors::C_WALL, sine);
					break;
				case TileID::HERO:
					*pixels++ = additive(Colors::C_HERO, sine);
					break;
				case TileID::ENEMY:
				case TileID::BOSS:
					*pixels++ = additive(Colors::C_ENEMY, sine);
					break;
				case TileID::CHEST:
					*pixels++ = additive(Colors::C_CHEST, sine);
					break;
				case TileID::HPOT:
					*pixels++ = additive(Colors::C_HPOT, sine);
					break;
			}
		}
	}

	pixels -= DUNGEON_SIZE;
	unlockTexture();
	renderPresent();
}
