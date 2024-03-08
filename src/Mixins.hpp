/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

class Mixin_CombatTurns {

public:
	int dieRoll{};
	bool goingFirst{};
	bool combatDone{};
};

class Mixin_CombatStats {
protected:
	int healthMax{};
	int healthCur{};
	int armour{};
	int damage{};

public:
	int getHealthMax() const { return healthMax; }
	int getHealthCur() const { return healthCur; }
	int getArmour() const { return armour; }
	int getDamage() const { return damage; }
};

class Mixin_Positional {
protected:
	int posX{};
	int posY{};

public:
	int getPosX() const { return posX; }
	int getPosY() const { return posY; }
};
