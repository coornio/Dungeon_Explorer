/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"
#include "Mixins.hpp"

enum ItemID {
	UNKNOWN = 0,
	ARMOUR  = 1,
	WEAPON  = 2,
	POTION  = 3,
};

class Item {
	using wstrv = std::wstring_view;
public:
	wstrv getName()  const { return itemName; }
	int   getID()    const { return itemID; }
	int   getPower() const { return itemPower; }
	int   getPosX()  const { return posX; }
	int   getPosY()  const { return posY; }

	virtual void encounterMessage()    const = 0;
	virtual void acquiredMessage(bool) const = 0;

protected:
	explicit Item(
		const int Y,
		const int X,
		const int id,
		const wstrv name,
		const int power
	)
		: posY(Y)
		, posX(X)
		, itemID(id)
		, itemName(name)
		, itemPower(power)
	{}

	std::wstring itemName;
	int itemID;
	int itemPower;
	int posX, posY;
};

class Treasure : public Item {
	using wstrv = std::wstring_view;
public:
	void acquiredMessage(bool) const override {}
	void encounterMessage() const override;
	explicit Treasure(const int Y, const int X)
		: Item(Y, X, ItemID::UNKNOWN, L""sv, 0)
	{}
};

class Armour : public Item {
	using wstrv = std::wstring_view;
	std::wstring itemDesc;
public:
	void acquiredMessage(bool equipable) const override;
	void encounterMessage() const override {}
	explicit Armour(
		const wstrv name,
		const int   power,
		const wstrv desc
	)
		: Item(-1, -1, ItemID::ARMOUR, name, power)
		, itemDesc(desc)
	{}
};

class Weapon : public Item {
	using wstrv = std::wstring_view;
	std::wstring itemDesc;
public:
	void acquiredMessage(bool equipable) const override;
	void encounterMessage() const override {}
	explicit Weapon(
		const wstrv name,
		const int   power,
		const wstrv desc
	)
		: Item(-1, -1, ItemID::WEAPON, name, power)
		, itemDesc(desc)
	{}
};

class Potion : public Item {
	using wstrv = std::wstring_view;
	std::wstring itemDesc;
public:
	void acquiredMessage(bool) const override;
	void encounterMessage() const override;
	Potion(const int Y, const int X, const int power)
		: Item(Y, X, ItemID::POTION, L"Healing Poultice"sv, power)
		, itemDesc(L"Its effects are questionable, but it should be of help."sv)
	{}
};
