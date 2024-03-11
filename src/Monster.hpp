/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"
#include "Mixins.hpp"

class Player;

class Enemy
	: public Mixin_CombatTurns
	, public Mixin_CombatStats
	, public Mixin_PositionData
{
public:
	int getDormancy() const { return dormant; }
	
	void attack(Player&);
	Enemy(const int Y, const int X, const bool boss)
		: Mixin_PositionData(Y, X)
		, dormant(boss)
	{}

	virtual void encounterMessage() const = 0;
	virtual void beDamaged(int)           = 0;
	virtual ~Enemy() {};

protected:
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
