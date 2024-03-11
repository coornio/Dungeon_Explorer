/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

class Mixin_CombatTurns {
protected:
	~Mixin_CombatTurns() = default;
	explicit Mixin_CombatTurns() = default;

public:
	int  dieRoll{};
	bool goingFirst{};
	bool combatDone{};
};

class Mixin_CombatStats {
protected:
	int healthMax;
	int healthCur;
	int armour;
	int damage;

	~Mixin_CombatStats() = default;
	explicit Mixin_CombatStats(
		int HM = 1,
		int HC = 1,
		int A = 0,
		int D = 0
	)
		: healthMax(HM)
		, healthCur(HC)
		, armour(A)
		, damage(D)
	{}

public:
	virtual int getHealthMax() const { return healthMax; }
	virtual int getHealthCur() const { return healthCur; }
	virtual int getArmour() const { return armour; }
	virtual int getDamage() const { return damage; }
};

class Mixin_PositionData {
protected:
	int posX;
	int posY;

	~Mixin_PositionData() = default;
	explicit Mixin_PositionData(int Y = 0, int X = 0)
		: posY(Y)
		, posX(X)
	{}

public:
	void setPosition(const int Y, const int X) {
		posY = Y;
		posX = X;
	}
	virtual int getPosX() const { return posX; }
	virtual int getPosY() const { return posY; }
};

class Mixin_ItemStats {
	using wsv = std::wstring_view;
public:
	enum class ID : int {
		ARMOUR,
		WEAPON,
		POTION,

		UNKNOWN = -1,
		NO_TYPE = -2,
	};

protected:
	ID  itemType;
	int itemPower;
	std::wstring itemName;
	std::wstring itemDesc;

	~Mixin_ItemStats() = default;
	explicit Mixin_ItemStats(
		const ID  id,
		const wsv name,
		const int power = 0,
		const wsv desc = L""sv
	)
		: itemType(id)
		, itemName(name)
		, itemPower(power)
		, itemDesc(desc)
	{}

public:
	virtual ID  getID()    const { return itemType; }
	virtual int getPower() const { return itemPower; }
	virtual wsv getName()  const { return itemName; }
	virtual wsv getDesc()  const { return itemDesc; }
};
