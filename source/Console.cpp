#include <nds.h>

#include "Console.h"

#include "Console_Font.h"
#include "ExtBgCtrl.h"

#define CONSOLE_CHARWIDTH 7
#define CONSOLE_CHARHEIGHT 13

#define CONSOLE_INVSCALEX (((8 << 9) / CONSOLE_CHARWIDTH + 1) >> 1)
#define CONSOLE_INVSCALEY (((16 << 9) / CONSOLE_CHARHEIGHT + 1) >> 1)

#define CONSOLE_NUMCOLUMNS 35
#define CONSOLE_NUMROWS 14

#define CONSOLE_MARGIN_LEFT ((256 - CONSOLE_NUMCOLUMNS * CONSOLE_CHARWIDTH) / 2)
#define CONSOLE_MARGIN_TOP ((192 - CONSOLE_NUMROWS * CONSOLE_CHARHEIGHT) / 2)

#define TABWIDTH 4

static const u16 textColours[16] = {
	RGB15(0,0,0),    RGB15(21,0,0),
	RGB15(0,21,0),   RGB15(21,11,0),
	RGB15(0,0,21),   RGB15(21,0,21),
	RGB15(0,21,21),  RGB15(21,21,21),
	RGB15(11,11,11), RGB15(31,11,11),
	RGB15(11,31,11), RGB15(31,31,11),
	RGB15(11,11,31), RGB15(31,11,31),
	RGB15(11,31,31), RGB15(31,31,31)
};

void Console::initialise(CONSOLE_RESERVES reserves) {

	initialise(reserves, 6, 0);
}

void Console::initialise(CONSOLE_RESERVES reserves, u32 mapBase, u32 tileBase) {

	bgCtrl.initialise(reserves == CONSOLE_SUB_H ? EXTBGCTRL_SUB_BG3 : EXTBGCTRL_MAIN_BG3, EXTBGCTRL_TILE_64x64);

	bgCtrl.setMapBase(mapBase);
	bgCtrl.setTileBase(tileBase);

	// the 32/256-dot horizontal correction is to centre the region of minimum
	// error (due to inexact scaling) in the middle of the screen
	bgCtrl.setOffset((CONSOLE_MARGIN_LEFT << 8) + 32, CONSOLE_MARGIN_TOP << 8);
	bgCtrl.setScale(CONSOLE_INVSCALEX, CONSOLE_INVSCALEY);

	this->reserves = reserves;
	colour = CONSOLE_WHITE;
}

void Console::activate() {

	// font data
	swiFastCopy(Console_Font_Tiles, bgCtrl.tileData(), (sizeof(Console_Font_Tiles) >> 2) | COPY_MODE_WORD);

	// palettes
	(reserves == CONSOLE_SUB_H ? BG_PALETTE_SUB : BG_PALETTE)[0] = 0;
	switch (reserves) {

		case CONSOLE_MAIN_E:
		vramSetBankE(VRAM_E_LCD);
		for (int j = 0; j < 16; j++) {
			VRAM_E_EXT_PALETTE[3][j][1] = textColours[j];
		}
		swiDelay(1); // see NOTE at end of file
		for (int j = 0; j < 16; j++) {
			VRAM_E_EXT_PALETTE[3][j][1] = textColours[j];
		}
		vramSetBankE(VRAM_E_BG_EXT_PALETTE);
		break;

		case CONSOLE_MAIN_F:
		vramSetBankF(VRAM_F_LCD);
		for (int j = 0; j < 16; j++) {
			VRAM_F_EXT_PALETTE[1][j][1] = textColours[j];
		}
		swiDelay(1); // see NOTE at end of file
		for (int j = 0; j < 16; j++) {
			VRAM_F_EXT_PALETTE[1][j][1] = textColours[j];
		}
		vramSetBankF(VRAM_F_BG_EXT_PALETTE_SLOT23);
		break;

		case CONSOLE_MAIN_G:
		vramSetBankG(VRAM_G_LCD);
		for (int j = 0; j < 16; j++) {
			VRAM_G_EXT_PALETTE[1][j][1] = textColours[j];
		}
		swiDelay(1); // see NOTE at end of file
		for (int j = 0; j < 16; j++) {
			VRAM_G_EXT_PALETTE[1][j][1] = textColours[j];
		}
		vramSetBankG(VRAM_G_BG_EXT_PALETTE_SLOT23);
		break;

		case CONSOLE_SUB_H:
		vramSetBankH(VRAM_H_LCD);
		for (int j = 0; j < 16; j++) {
			VRAM_H_EXT_PALETTE[3][j][1] = textColours[j];
		}
		swiDelay(1); // see NOTE at end of file
		for (int j = 0; j < 16; j++) {
			VRAM_H_EXT_PALETTE[3][j][1] = textColours[j];
		}
		vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
		break;
	}

	clear();

	// display control
	(reserves == CONSOLE_SUB_H ? REG_DISPCNT_SUB : REG_DISPCNT) = MODE_3_2D | DISPLAY_BG3_ACTIVE | DISPLAY_BG_EXT_PALETTE;

	bgCtrl.redraw();
}

void Console::deactivate() {
}

void Console::clear() {

	for (int i = 0; i < CONSOLE_NUMROWS * 128; i++) {
		bgCtrl.mapData()[i] = 0;
	}
	cursorRow = cursorColumn = 0;
}

void Console::setCursor(int row, int column) {

	cursorRow = row;
	cursorColumn = column;
}

void Console::print(CONSOLE_COLOUR colour) {

	this->colour = colour;
}

void Console::print(const char *string) {

	for (char *scanner = (char *)string; *scanner != '\0'; scanner++) {
		print(*scanner);
	}
}

void Console::printWrap(const char *string) {

	char *scanner = (char *)string;
	while (true) {

		char *wordEnd = scanner;
		while (*wordEnd != '\0' && *wordEnd != ' ' && *wordEnd != '\n') {
			wordEnd++;
		}

		if (cursorColumn < CONSOLE_NUMCOLUMNS && cursorColumn != 0) {
			if (cursorColumn + wordEnd - scanner + 1 > CONSOLE_NUMCOLUMNS) {
				print('\n');
			}
			else {
				print(' ');
			}
		}

		while (scanner < wordEnd) {
			print(*(scanner++));
		}

		if (*wordEnd == '\n' && cursorColumn < CONSOLE_NUMCOLUMNS) {
			print('\n');
		}
		else if (*wordEnd == '\0') break;

		scanner++;
	}
}

void Console::print(char c) {

	// line wrap
	if (cursorColumn >= CONSOLE_NUMCOLUMNS) {
		cursorColumn = 0;
		cursorRow++;
	}

	// shift up one line
	if (cursorRow == CONSOLE_NUMROWS) {
		u16 *mapRam = bgCtrl.mapData();
		for (int i = 0; i < (CONSOLE_NUMROWS - 1) * 128; i++) {
			mapRam[i] = mapRam[i + 128];
		}
		for (int i = (CONSOLE_NUMROWS - 1) * 128; i < CONSOLE_NUMROWS * 128; i++) {
			mapRam[i] = 0;
		}
		cursorRow--;
	}

	switch (c) {

		case '\t':
		cursorColumn = (cursorColumn / TABWIDTH + 1) * TABWIDTH;
		break;

		case '\n':
		cursorRow++;
		case '\r':
		cursorColumn = 0;
		break;

		default:
		{
			u16 index = (u16)(c - 0x20);
			if (index > 95) return;
			index |= colour << 12;
			bgCtrl.mapData()[(cursorRow << 7) + cursorColumn] = index;
			bgCtrl.mapData()[(cursorRow << 7) + cursorColumn + 64] = index + 96;
			cursorColumn++;
		}
	}
}

void Console::print(int value) {

	char buffer[12];

	char *start = buffer + 12;
	*start = '\0';
	if (value < 0) {
		do {
			*(--start) = '0' - value % 10;
			value /= 10;
		} while (value < 0);
		*(--start) = '-';
	}
	else {
		do {
			*(--start) = '0' + value % 10;
			value /= 10;
		} while (value > 0);
	}

	print(start);
}

void Console::printHex(u32 value, int width) {

	static const char digits[] = "0123456789abcdef";
	char buffer[width];

	char *start = buffer + width;
	*start = '\0';
	do {
		*(--start) = digits[value & 0xf];
		value >>= 4;
	} while (start > buffer);

	print(buffer);
}

void Console::printBin(u32 value, int width) {

	char buffer[width];

	char *start = buffer + width;
	*start = '\0';
	do {
		*(--start) = '0' + (value & 1);
		value >>= 1;
	} while (start > buffer);

	print(buffer);
}

// NOTE:
// I CANNOT FIGURE OUT WHY, BUT IT IS NECESSARY TO WRITE TWICE WITH A
// FUNCTION OR BIOS CALL IN BETWEEN, FOR THE WRITES TO BE EFFECTIVE
