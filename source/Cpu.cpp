#include <nds.h>

#include "Cpu.h"
#include "Kbd.h"
#include "EmuScreen.h"

Cpu CPU;

void Cpu::tick() {

	mibicycles_last_tick = mibicycles_planned - mibicycles_due;

	// Schedule cycles unless overloaded
	if (mibicycles_due < mibicycles_per_tick) {
		mibicycles_due += mibicycles_per_tick;
	}

	mibicycles_planned = mibicycles_due;
}

void Cpu::run() {

	while (mibicycles_due > 0) {

		// Fetch
		u16 inst = rom[pc++];

		if (inst & (1 << 15)) {

			// C instruction

			// Detect reads from the keyboard register
			bool keyboard_read = false;

			u16 opx = rd;
			if (inst & (1 << 11)) opx = 0;
			if (inst & (1 << 10)) opx = ~opx;

			u16 opy = ra;
			if (inst & (1 << 12)) {
				if (ra < (16 << 10)) opy = ram[ra];
				else if (ra < (24 << 10)) {
					opy = EmuScreen::read(ra - (16 << 10));
				}
				else if (ra == (24 << 10)) {
					opy = KBD.read();
					keyboard_read = true;
				}
			}
			if (inst & (1 << 9)) opy = 0;
			if (inst & (1 << 8)) opy = ~opy;

			u16 res;
			if (inst & (1 << 7)) res = opx + opy;
			else res = opx & opy;
			if (inst & (1 << 6)) res = ~res;

			if ((inst & (1 << 0)) && !(res & (1 << 15)) && res) pc = ra;
			else if ((inst & (1 << 1)) &&             res == 0) pc = ra;
			else if ((inst & (1 << 2)) &&    (res & (1 << 15))) pc = ra;

			if (inst & (1 << 3)) {
				if (ra < (16 << 10)) ram[ra] = res;
				else if (ra < (24 << 10)) {
					EmuScreen::write(ra - (16 << 10), res);
				}
			}
			if (inst & (1 << 5)) ra = res;
			if (inst & (1 << 4)) rd = res;

			// Attempt to synchronise frames with keyboard register reads
			// (Overdue cycles are postponed until the next tick)
			if (keyboard_read && mibicycles_due < mibicycles_per_tick) break;
		}
		else {

			// A instruction
			ra = inst;
		}

		// One cycle elapsed
		mibicycles_due -= 1024;
	}
}
