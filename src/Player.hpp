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

class Player final {
	int healthMax{ 40 };
	int healthCur{ 40 };
	int armour{ 0 };
	int damage{ 0 };
	int posX{}, posY{};
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

	int getHealthMax() const { return healthMax; }
	int getHealthCur() const { return healthCur; }
	int getArmour() const { return armour; }
	int getDamage() const { return damage; }
	int getPosX() const { return posX; }
	int getPosY() const { return posY; }

	void introduction() const;
	void defeated() const;



	void setPosition(const int Y, const int X);

	void beDamaged(int);
	void attack(Monster&);
	Player();
};
