/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"
#include "Mixins.hpp"


class Monster;
class Item;

class Player final
	: public Mixin_CombatTurns
	, public Mixin_CombatStats
	, public Mixin_PositionData
{
	Well512 rnd;

	const std::wstring name{ L"Hero"sv };
	objVector<Item> belt{};

	bool setArmour(const int power);
	bool setDamage(const int power);

	template<typename Obj>
	void equipDirectGear(Obj&& item) {
		canEquip(belt.insertEntry(std::forward<Obj>(item)));
	}

public:
	void takeItem(const std::size_t, objVector<Item>&, const bool);
	bool canEquip(const Item&);

	void introduction() const;
	void defeated() const;

	void beDamaged(int);
	void attack(Monster&);
	Player(const int = 0, const int = 0);
};
