#include "EmuScreen.h"

#include <nds.h>

volatile u16 EmuScreen::vram[8 << 10];
volatile bool EmuScreen::dirty_line[128];

int EmuScreen::bg_id;

#define GET_BIT(x, i) (((x) >> i) & 1)

void EmuScreen::set_main() {

	videoSetMode(MODE_3_2D); // Main engine layers: {0: T, 1: T, 2: T, 3: E}

	bg_id = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

	// Generate a palette approximating four black/white dots per pixel
	// The NDS LCD dots are in BGR order from left to right
	for (int i = 0; i < 16; i++) {
		int l = GET_BIT(i, 0) + GET_BIT(i, 2);
		int r = GET_BIT(i, 1) + GET_BIT(i, 3);
		BG_PALETTE[i] = RGB15(31 * r / 2, 31 * (l + r) / 4, 31 * l / 2);
	}

	bgSetScroll(bg_id, 0, -32);
	bgUpdate(); // Apply scroll vector

	bgShow(bg_id);

	for (int i = 0; i < (8 << 10); i++) vram[i] = 0;
	for (int i = 0; i < 128; i++) dirty_line[i] = true;
}

void EmuScreen::draw() {

	for (int i = 0; i < 128; i++) {

		if (dirty_line[i]) {
			dirty_line[i] = false;

			for (int j = 0; j < 32; j++) {

				vu16 *source = vram + (i << 6) + j;

				u16 top_dots = ~source[0];
				u16 btm_dots = ~source[32];

				u16 evn_pixels = (top_dots & 0x3333) | ((btm_dots & 0x3333) << 2);
				u16 odd_pixels = (btm_dots & 0xcccc) | ((top_dots & 0xcccc) >> 2);

				u16 *target = bgGetGfxPtr(bg_id) + (i << 7) + (j << 2);

				for (int i = 0; i < 4; i++) {
					target[i] = (evn_pixels & 0xf) | ((odd_pixels & 0xf) << 8);
					evn_pixels >>= 4;
					odd_pixels >>= 4;
				}
			}
		}
	}
}
