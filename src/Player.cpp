/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Item.hpp"
#include "Player.hpp"
#include "Monster.hpp"

void Player::beDamaged(const int points) {
	if (points <= armour) return;
	else healthCur -= points - armour;
}

void Player::attack(Monster& monster) {
	monster.beDamaged((rnd() % damage - 1) + 1);
}

void Player::takeItem(
	const std::size_t idx,
	objVector<Item>& vec,
	const bool replace
) {
	if (!replace) {
		belt.pullEntry(idx, vec);
		return;
	}

	for (std::size_t i{}; i < belt.size(); ++i) {
		if (belt[i].getID() == vec[idx].getID()) {
			belt.deleteEntry(i);
			belt.pullEntry(idx, vec);
			return;
		}
	}
}

bool Player::canEquip(const Item& item) {
	switch (item.getID()) {
		case Item::ID::ARMOUR: return setArmour(item.getPower());
		case Item::ID::WEAPON: return setDamage(item.getPower());
		case Item::ID::POTION: return true;
		default: return false;
	}
}

bool Player::setArmour(const int power) {
	if (power > armour) {
		armour = power;
		return true;
	}
	return false;
}

bool Player::setDamage(const int power) {
	if (power > damage) {
		damage = power;
		return true;
	}
	return false;
}

Player::Player(const int Y, const int X)
	: Mixin_CombatStats(40, 40)
	, Mixin_PositionData(Y, X)
{
	introduction();

	equipDirectGear(
		Weapon(L"Combat Knife"sv, 3,
		L"Custom-made with a serrated blade. Useful for survival."sv)
	);
	equipDirectGear(
		Armour(L"Tattered Armour"sv, 1,
		L"Custom-made flexible armour, now in complete shambles."sv)
	);
	/*for (auto a{ 0 }; a < belt.size(); ++a) {
		std::wcout << a << L": "sv << belt[a].getName() << L'\n';
	}
	std::wcout << L"Damage: " << damage << std::endl;
	std::wcout << L"Armour: " << armour << std::endl;*/
}

void Player::introduction() const {
	textBlock(std::wstringstream{}
		<< L"Ugh.. your head is pounding. You remember the ground collapsing below your very feet, but the rest is a blur. You probably blacked out on the descent."sv
		<< L" \n \n "sv
		<< L"The place is pitch black, cold, and humid. Thankfully your enchanted amulet helps in lighting up your immediate surroundings."sv
		<< L" \n \n "sv
		<< L"Your armor is tattered, dented, and barely hanging on. It's doubtful it'll provide any meaningful protection in this state. For what it's worth, your knife survived the fall and is still usable. If the distant sounds are of any indication, this place isn't at all hospitable."sv
		<< L" \n \n "sv
		<< L"Worrisome as your circumstances are, you can feel a breeze so there must be an exit somewhere you could reach. You steele yourself and venture out to explore the cavernous paths. This is not where your story ends!"sv
	);
}

void Player::defeated() const {
	textBlock(std::wstringstream{}
		<< L"..... "sv
		<< L" \n \n "sv
		<< L"Immense pain wracks your very being, your eyes bloodshot, struggling to even breathe. Your gaze shifts downwards as your head tilts in weakness, blood spewing freely from your mouth."sv
		<< L" \n \n "sv
		<< L"The monster's appendage has pierced clean through whatever protection you had, impaling your body and lifting it high in the air."sv
		<< L" \n \n "sv
		<< L"Your eyelids are so heavy. You choke and gurgle, but you have no strength to struggle anymore. As your life seeps away from your body, the last thing you feel is the monstrosity's jaws clamping down on your body, then it's over."sv
		<< L" \n \n "sv
		<< L"You have died."sv
	);
}
