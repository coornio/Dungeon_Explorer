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
		case TileID::WALL:
			return;
		default:
			map[curY][curX] = TileID::PATH;
			map[newY][newX] = TileID::HERO;
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

bool Dungeon::readMapFile() {
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

			auto tile{ as<TileID>(ch) };
			map[Y][X] = tile;

			switch (tile) {
				case TileID::HERO:
					hero.setPosition(Y, X);
					break;
				case TileID::ENEMY:
					monsters.createEntry<Monster>(Y, X, false);
					break;
				case TileID::BOSS:
					monsters.createEntry<Monster>(Y, X, true);
					break;
				case TileID::CHEST:
					treasure.createEntry<Treasure>(Y, X);
					break;
				case TileID::HPOT:
					healpots.createEntry<Potion>(Y, X, (rng() % 31) + 10);
					break;
			}
		}
	}
	return true;
}


