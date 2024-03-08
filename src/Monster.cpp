/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Item.hpp"
#include "Monster.hpp"
#include "Player.hpp"

void Enemy::attack(Player& player) {
	player.beDamaged((rnd() % damage - 1) + 1);
}

void Monster::encounterMessage() const {
	textBlock(std::wstringstream{}
		<< L"Encountered an unknown monstrosity!"sv
		<< L" \n \n "sv
		<< (dormant
			? L"There's something ominous about this one. You're not sure what it is, but you feel like you could lose your life at any moment."sv
			: L"Its disfigured visage and corrupted form towers over you, and the sounds it's producing could only be described as unholy. Be on your guard!"sv
		)
	);
};

void Monster::beDamaged(int points) {
	points -= armour;
	if (points <= 0) return;

	healthCur -= points - armour;
}

Monster::Monster(const int Y, const int X, bool boss)
	: Enemy(Y, X, boss)
{
	healthMax = healthCur = (rnd() % 11) + 20;
	armour = (rnd() % 3) + 2;
	damage = (rnd() % 3) + 4;
	std::wcout
		<< "  Health: " << healthCur
		<< ", Armour: " << armour
		<< ", Damage: " << damage
		<< ", Dormant: " << dormant
		<< std::endl;
}

//-----------------------------------------------//

void Boss::beDamaged(int points) {
	points -= armour;
	if (points <= 0) return;

	healthCur -= points - armour;
}

Boss::Boss(const int Y, const int X, bool boss)
	: Enemy(Y, X, boss)
{
	healthMax = healthCur = (rnd() % 11) + 20;
	armour = (rnd() % 3) + 2;
	damage = (rnd() % 3) + 4;
	std::wcout
		<< "  Health: " << healthCur
		<< ", Armour: " << armour
		<< ", Damage: " << damage
		<< ", Dormant: " << dormant
		<< std::endl;
}
