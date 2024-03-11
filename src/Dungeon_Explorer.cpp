/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "Includes.hpp"
#include "Dungeon.hpp"

/*

Ε) Τρία κανονικά τέρατα (class) τα οποία να έχουν τα ακόλουθα στατιστικά (τυχαία)
	Life (20-25-30)
	Armor (2-3-4)
	Damage (1d4-1d5-1d6)
(1 μονάδα)

ΣΤ) Ένα Boss monster (στο τέλος) που να:
   Ξεκινά σαν τέρας του τύπου (Δ) και να μετασχηματίζεται όταν φτάσει στο 20%:
   BOSS: Life 60, Armor 5, Damage 1d12
(3 μονάδες)

Μόλις ο ήρωας συναντήσει κάποιο τέρας, να γίνεται μία turn based μάχη ως ακολούθως:
   α) Το τέρας και ο ήρωας ρίχνουν initiative (ένα ζάρι ό,τι θέλετε και παίζει πρώτος αυτός
	που θα φέρει το μεγαλύτερο)
   β) Ο πρώτος θα κάνει το action του, και μετά ο δεύτερος, και το initiative θα επαναληφθεί.
   γ) Αυτός που παίζει, αν είναι τέρας έχει μόνο attack action. Αν είναι ήρωας, έχει τις ακόλουθες
	  actions: Attack - Use.
   δ) Attack action: Κάθε επίθεση γίνεται resolved με τον ακόλουθο τύπο. Ζημιά στον αντίπαλο = με
	όπλο - armor του αντιπάλου.
   ε) Το use χρησιμεύει στην περίπτωση που ο ήρωας θέλει να χρησιμοποιήσει κάποιο healing potion που έχει στο inventory του.

   Εννοείται, πως αν βρει κάποιο καλύτερο όπλο ή armor, τα φορά αυτόματα.
	Το παιχνίδι τελειώνει με την ήττα του Boss.
*/

int SDL_main(int, char**) {

	std::cout << "\33[?25l"; // hide caret line
	std::setlocale(LC_ALL, "");

	static bool newgame{};
	static bool waitkey{};
	SDL_Event      event;
	RenderSettings Render;
	FrameLimiter   Delay(10.0 / 3.0, false);
	using namespace bic;

	if (!Render.createWindow())   goto exit;
	if (!Render.createRenderer()) goto exit;
	if (!Render.createTexture())  goto exit;

restart:
	kb.updateCopy();
	do {
		Dungeon dungeon;
		if (!dungeon.readMapFile())
			goto exit;

		while (true) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT)
					goto exit;
			}
			if (kb.isPressed(KEY(ESCAPE)))
				goto exit;
			if (kb.isPressed(KEY(BACKSPACE)))
				goto restart;
			if (dungeon.inCombat) {
				//goto skip;
				switch (dungeon.currentPhase) {
					case Dungeon::Phase::ROLL: {

					} break;
					case Dungeon::Phase::BATTLE: {

					} break;
					case Dungeon::Phase::STANDBY: {

					} break;
					case Dungeon::Phase::END: {

					} break;
					default: dungeon.inCombat = false;
				}
			} else {
				//                                       actually  Y   X
				if (kb.isPressed(KEY(W))) dungeon.updatePlayerPos(-1,  0);
				if (kb.isPressed(KEY(A))) dungeon.updatePlayerPos( 0, -1);
				if (kb.isPressed(KEY(S))) dungeon.updatePlayerPos( 1,  0);
				if (kb.isPressed(KEY(D))) dungeon.updatePlayerPos( 0,  1);
			}

			//skip:
			kb.updateCopy();
			Render.flushDisplay(
				dungeon,
				dungeon.hero.getPosY(),
				dungeon.hero.getPosX()
			);
		}
	} while (false);

exit:
	if (newgame) {
		newgame = false;
		goto restart;
	}

	Render.quit();
	return 0;
}
