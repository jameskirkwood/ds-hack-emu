#ifndef EMUSCREEN_H
#define EMUSCREEN_H

#include <nds.h>

// Emulated Hardware: Screen
//-----------------------------------------------------------------------------

class EmuScreen {

private:

	static volatile u16 vram[256 * 32];
	static volatile bool dirty_line[128];

	static int bg_id;

public:

// Application interface
//-----------------------------------------------------------------------------

	// Activate on main display
	// - Requires 128 kB mapped at the base of main background memory
	static void set_main();

	// Render screen to display
	static void draw();

// Virtual hardware interface
//-----------------------------------------------------------------------------

	// Read and write emulated video memory
	static u16 read(u16 address) {
		return vram[address];
	}
	static void write(u16 address, u16 value) {
		dirty_line[address >> 6] = true;
		vram[address] = value;
	}
};

#endif
