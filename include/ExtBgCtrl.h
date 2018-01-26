#ifndef EXTBGCTRL_H
#define EXTBGCTRL_H

#include <nds.h>

// DESCRIPTION:
//   Extended Background Controller
//
// RESERVES:
//   * Specified background control register of specified engine
//   * Corresponding affine transform registers
//   (but not the VRAM used for the map, tiles, bitmap or palettes)
//
// REQUIREMENTS:
//   Ensure that the display is set to use the assigned background as an
//   Extended background before activating and while active.

// DETAILED DESCRIPTION:
//
// An ExtBgCtrl object must be initialised on a given background register
// (2 or 3) of a given 2D graphics engine (main or sub). The format specified
// at initialisation determines whether the background is in tile, indexed
// bitmap or direct colour bitmap mode, and the dimensions (in tiles or pixels,
// as relevant) of the raw image.
//
// The map base (aka bmp base in bitmap modes), tile base, priority, mosaic
// effect, wrapping, position and affine transformation matrix can be set.
// The background control, origin and matrix registers will be updated upon
// redraw if the object is active.
//
// Helper methods are provided to obtain pointers to the map data, tile data
// and base palette currently used by the activity, (but not reserved by it).

typedef enum EXTBGCTRL_RESERVES {
	EXTBGCTRL_MAIN_BG2,
	EXTBGCTRL_MAIN_BG3,
	EXTBGCTRL_SUB_BG2,
	EXTBGCTRL_SUB_BG3
} EXTBGCTRL_RESERVES;

typedef enum EXTBGCTRL_FORMAT {
	EXTBGCTRL_TILE_16x16,
	EXTBGCTRL_TILE_32x32,
	EXTBGCTRL_TILE_64x64,
	EXTBGCTRL_TILE_128x128,
	EXTBGCTRL_INDEXED_128x128,
	EXTBGCTRL_INDEXED_256x256,
	EXTBGCTRL_INDEXED_512x256,
	EXTBGCTRL_INDEXED_512x512,
	EXTBGCTRL_DIRECT_128x128,
	EXTBGCTRL_DIRECT_256x256,
	EXTBGCTRL_DIRECT_512x256,
	EXTBGCTRL_DIRECT_512x512
} EXTBGCTRL_FORMAT;

class ExtBgCtrl {

public:

	void initialise(EXTBGCTRL_RESERVES reserves, EXTBGCTRL_FORMAT format);

	void redraw();

	// background control

	void setMapBase(u32 mapBase);
	void setTileBase(u32 tileBase);

	void setPriority(u32 priority);

	void setMosaic(bool mosaic);
	void setWrap(bool wrap);

	void setAnchor(s32 x, s32 y);
	void setOffset(s32 x, s32 y);
	void addOffset(s32 x, s32 y);
	void setScale(s16 x, s16 y);
	void setMatrix(s32 scaleX, s32 scaleY, s32 shearX, s32 shearY);

	// helpers

	u16 *mapData();
	u16 *tileData();

	u16 *basePalette();

private:

	EXTBGCTRL_RESERVES reserves;

	// background control

	u8 mapBase;
	u8 tileBase;

	u8 priority;

	EXTBGCTRL_FORMAT format;

	bool mosaic;
	bool wrap;

	s32 anchorX;
	s32 anchorY;

	s32 offsetX;
	s32 offsetY;

	s16 scaleX;
	s16 scaleY;

	s16 shearX;
	s16 shearY;
};

#endif
