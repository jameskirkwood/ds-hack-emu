#include <nds.h>

#include "Vid.h"
#include "Cpu.h"

Vid VID;

void Vid::init() {

	videoSetMode(MODE_3_2D);      // Main engine layers: {0: T, 1: T, 2: T, 3: E}
	vramSetBankA(VRAM_A_MAIN_BG); // 128 kB at base of main background memory
	lcdMainOnTop();
	powerOn(POWER_ALL_2D);

	bg_id = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

	// Generate a palette approximating four black/white dots per pixel
	// The NDS LCD dots are in BGR order from left to right
	for (int i = 0; i < 16; i++) {
		int l = 0;
		int r = 0;
		if (i & 1) l++;
		if (i & 2) r++;
		if (i & 4) l++;
		if (i & 8) r++;
		BG_PALETTE[i] = RGB15(31 * r / 2, 31 * (l + r) / 4, 31 * l / 2);
	}

	bgSetScroll(bg_id, 0, -32);
	bgUpdate(); // Apply scroll vector

	bgShow(bg_id);

	for (int i = 0; i < (8 << 10); i++) vram_old[i] = ~0;
}

void Vid::draw() {

	frame_strips = 0;

	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 32; j++) {

			u16 offset = (i << 6) + j;
			u16 *old = vram_old + offset;
			u16 *cur = CPU.ram + (16 << 10) + offset;

			if (cur[0] != old[0] || cur[32] != old[32]) {

				frame_strips++;

				old[0]  = cur[0];
				old[32] = cur[32];

				u16 lo = ~cur[0];
				u16 hi = ~cur[32];

				u16 los = (lo & 0x3333) | ((hi & 0x3333) << 2);
				u16 his = (hi & 0xcccc) | ((lo & 0xcccc) >> 2);

				u16 *strip = bgGetGfxPtr(bg_id) + (i << 7) + (j << 2);
				for (int i = 0; i < 4; i++) {
					strip[i] = (los & 0xf) | ((his & 0xf) << 8);
					los >>= 4;
					his >>= 4;
				}
			}
		}
	}
}
