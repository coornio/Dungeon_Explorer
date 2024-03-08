/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"
#include "Mixins.hpp"

class Player;

class Enemy {
public:
	int getHealthMax() const { return healthMax; }
	int getHealthCur() const { return healthCur; }
	int getDormancy() const { return dormant; }
	int getArmour() const { return armour; }
	int getDamage() const { return damage; }
	int getPosX() const { return posX; }
	int getPosY() const { return posY; }
	
	void attack(Player&);
	Enemy(const int Y, const int X, const bool boss)
		: posY(Y)
		, posX(X)
		, dormant(boss)
	{}

	virtual void encounterMessage() const = 0;
	virtual void beDamaged(int)           = 0;
	virtual ~Enemy() {};

protected:
	int healthMax{};
	int healthCur{};
	int armour{};
	int damage{};
	int posX{}, posY{};
	const bool dormant;
	Well512 rnd;
};

class Monster final : public Enemy {

public:
	void encounterMessage() const override;
	void beDamaged(int) override;
	Monster(const int, const int, const bool = false);
};

class Boss final : public Enemy {

public:
	void encounterMessage() const override {}
	void beDamaged(int) override;
	Boss(const int, const int, const bool = false);
};
