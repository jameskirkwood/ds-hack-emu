#include <nds.h>

#include "Cpu.h"
#include "Kbd.h"
#include "EmuScreen.h"

Cpu CPU;

void Cpu::init() {

	pc = ra = rd = 0;
	cycles_due = 0;
}

void Cpu::tick() {

	cycles_due += cycles_per_frame;

	while (cycles_due-- > 0) {

		bool keyboard_read = false;

		u16 inst = rom[pc++];

		if (inst & (1 << 15)) {

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

			if (keyboard_read && cycles_due < cycles_per_frame) break;
		}
		else {
			ra = inst;
		}

		if (REG_VCOUNT == 191) break;
	}
}

void Cpu::setfreq(int f) {

	cycles_per_frame = f / 60;
}