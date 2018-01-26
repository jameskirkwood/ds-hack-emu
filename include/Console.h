#ifndef MY_CONSOLE_H
#define MY_CONSOLE_H

#include <nds.h>

#include "ExtBgCtrl.h"

// DESCRIPTION:
//   Colour Console Display Controller
//
// RESERVES:
//   * The assigned 2D graphics engine
//   * The assigned VRAM bank (E, F, or G on main, H on sub) (for the palette),
//   * The first 3.5kB (< 2 map bases) at the assigned map base (for the screen),
//   * The first 12kB (< 6 map bases) at the assigned tile base (for the font),
//   (but not the VRAM used for the map, tiles and extended palettes)
//
// REQUIREMENTS:
//   The main or sub engine must have had at least 16kB available at the
//   beginning of background graphics VRAM prior to activation on that engine,
//   if using the default map and tile bases.
//
// DETAILS:
//   supports all 95 printable ASCII characters, horizontal tabs, newlines and
//   carriage returns
//   supports 16 colours of text, automatic wrapping and numeric formatting
//   14 rows by 35 columns, 7 x 13 pixels per character
//   width 5px, cap height 8px, x-height 5px, descent 2px

// Resource Allocation
typedef enum CONSOLE_RESERVES {

	CONSOLE_MAIN_E,
	CONSOLE_MAIN_F,
	CONSOLE_MAIN_G,
	CONSOLE_SUB_H

} CONSOLE_RESERVES;

// Text Colours
typedef enum CONSOLE_COLOUR {

	CONSOLE_BLACK,  CONSOLE_DRED,
	CONSOLE_DGREEN, CONSOLE_BROWN,
	CONSOLE_DBLUE,  CONSOLE_DMGNTA,
	CONSOLE_DCYAN,  CONSOLE_LGREY,
	CONSOLE_DGREY,  CONSOLE_LRED,
	CONSOLE_LGREEN, CONSOLE_YELLOW,
	CONSOLE_LBLUE,  CONSOLE_LMGNTA,
	CONSOLE_LCYAN,  CONSOLE_WHITE,

	CONSOLE_RED   = CONSOLE_LRED,
	CONSOLE_GREEN = CONSOLE_LGREEN,
	CONSOLE_BLUE  = CONSOLE_LBLUE,
	CONSOLE_MGNTA = CONSOLE_LMGNTA,
	CONSOLE_CYAN  = CONSOLE_LCYAN,
	CONSOLE_GREY  = CONSOLE_DGREY

} CONSOLE_COLOUR;

class Console {

	CONSOLE_COLOUR colour;

public:

	void initialise(CONSOLE_RESERVES reserves);
	void initialise(CONSOLE_RESERVES reserves, u32 mapBase, u32 tileBase);

	void activate();   // reserve resources (call before any other methods)
	void deactivate(); // relinquish resources

	CONSOLE_RESERVES reserves;

	// clear the console, returning the cursor to 0:0
	void clear();

	// move the cursor to a specific position
	void setCursor(int row, int column);

	// set the colour to be used by the next printing operation
	void print(CONSOLE_COLOUR colour);

	// print all characters from a buffer
	// (text colour index (0 to 15) may optionally be specified)
	void print(const char *string);

	// print all characters from a buffer without breaking within words if possible
	// (text colour index (0 to 15) may optionally be specified)
	void printWrap(const char *string);

	// print a single character
	void print(char c);

	// print numbers in decimal, hexadecimal (lower case) or binary respectively
	// the number of digits must be specified for hexadecimal and binary printing
	// (text colour index (0 to 15) may optionally be specified)
	void print(int value);
	void printHex(u32 value, int width);
	void printBin(u32 value, int width);

	// background control
	ExtBgCtrl bgCtrl;

private:

	// cursor position on the screen
	int cursorRow;
	int cursorColumn;
};

// STL stream style printing
template <typename T>
inline Console& operator<<(Console& console, T t) {
	console.print(t);
	return console;
}

#endif
