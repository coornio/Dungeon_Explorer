/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Item.hpp"

void Treasure::encounterMessage() const {
	textBlock(std::wstringstream{}
		<< L"You stumble over an old, rusted strongbox sticking out of the ground as you walk in the dark!"sv
		<< L" \n \n "sv
		<< L"It's covered in soil and some sort of sticky substance, but thankfully it is rather easy to pry open using your knife and a bit of force."sv
	);
}

void Armour::acquiredMessage(bool equipable) const {
	textBlock(std::wstringstream{}
		<< L"You have acquired a "sv
		<< itemName << L"! \n "sv
		<< itemDesc << L" \n "sv
		<< L"It offers "sv << ANSI(L'm', 33)
		<< itemPower << ANSI(L'm', 0)
		<< L" points of protection."sv
		<< L" \n \n "sv
		<< (equipable
			? L"It will make for quite a nice upgrade compared to what you're currently wearing."sv
			: L"Unfortunately, its protection is visibly lackluster compared to your current wear and you are not in a position to be carrying extra weight."sv
		)
	);
}

void Weapon::acquiredMessage(bool equipable) const {
	textBlock(std::wstringstream{}
		<< L"You have acquired a "sv
		<< itemName << L"! \n "sv
		<< itemDesc << L" \n "sv
		<< L"It offers "sv << ANSI(L'm', 33)
		<< itemPower << ANSI(L'm', 0)
		<< L" points of offense."sv
		<< L" \n \n "sv
		<< (equipable
			? L"This should help you fight back more effectively and make it out alive."sv
			: L"Unfortunately, its offensive capabilities are an outright downgrade and in this ominous place you cannot afford to be handicapping yourself."sv
		)
	);
}


void Potion::acquiredMessage(bool) const {
	textBlock(std::wstringstream{}
		<< L"You have acquired a "sv
		<< itemName << L"! \n "sv
		<< itemDesc
	);
}
void Potion::encounterMessage() const {
	textBlock(std::wstringstream{}
		<< L"You spot the glint of a reflective surface near your feet!"sv
		<< L" \n \n "sv
		<< L"It appears to be a sealed, though cracked vial of sorts. The label is faded from wear and decomposition, but judging by the smell as you pop open the cork and give it a whiff, it is likely a healing poultice."sv
		<< L" \n \n "sv
		<< L"It appears to still be usable, though age and exposure likely had an effect on its effectiveness. You take the vial with you."sv
	);
}
