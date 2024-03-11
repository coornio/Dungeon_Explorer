/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "Includes.hpp"
#include "Mixins.hpp"

class Item
	: public Mixin_PositionData
	, public Mixin_ItemStats
{
	using wsv = std::wstring_view;
public:
	virtual void encounterMessage()    const = 0;
	virtual void acquiredMessage(bool) const = 0;

protected:
	explicit Item(
		const int Y,
		const int X,
		const ID  id,
		const wsv name,
		const int power,
		const wsv desc
	)
		: Mixin_PositionData(Y, X)
		, Mixin_ItemStats(id, name, power, desc)
	{}
};

class Treasure : public Item {
	using wsv = std::wstring_view;
public:
	void acquiredMessage(bool) const override {}
	void encounterMessage()    const override;

	explicit Treasure(const int Y, const int X)
		: Item(Y, X, ID::NO_TYPE, L""sv, 0, L""sv)
	{}
};

class Armour : public Item {
	using wsv = std::wstring_view;
public:
	void acquiredMessage(bool) const override;
	void encounterMessage()    const override {}

	explicit Armour(const wsv name, const int power, const wsv desc)
		: Item(-1, -1, ID::ARMOUR, name, power, desc)
	{}
};

class Weapon : public Item {
	using wsv = std::wstring_view;
public:
	void acquiredMessage(bool) const override;
	void encounterMessage()    const override {}

	explicit Weapon(const wsv name, const int power, const wsv desc)
		: Item(-1, -1, ID::WEAPON, name, power, desc)
	{}
};

class Potion : public Item {
public:
	void acquiredMessage(bool) const override;
	void encounterMessage()    const override;

	Potion(const int Y, const int X, const int power)
		: Item(Y, X, ID::POTION, L"Healing Poultice"sv, power,
			(L"Its effects are questionable, but it should be of help."sv))
	{}
};
