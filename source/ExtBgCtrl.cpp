#include <nds.h>

#include "ExtBgCtrl.h"

void ExtBgCtrl::initialise(EXTBGCTRL_RESERVES reserves, EXTBGCTRL_FORMAT format) {

	this->reserves = reserves;
	this->format = format;

	anchorX = anchorY = 0;
	offsetX = offsetY = 0;
	scaleX = scaleY = 1;
	shearX = shearY = 0;
	wrap = mosaic = false;
	priority = 0;
}

void ExtBgCtrl::redraw() {

	u32 controlBits = 0;
	controlBits |= ((mapBase & 31) << 8);
	controlBits |= (format > 3 ? BIT(7) : (tileBase & 15) << 2);
	controlBits |= (format > 7 ? BIT(2) : 0);
	controlBits |= ((format & 3) << 14);
	controlBits |= (priority & 3);
	controlBits |= (mosaic ? BIT(6) : 0);
	controlBits |= (wrap ? BIT(13) : 0);

	bg_transform transform = {
		scaleX, shearY,
		shearX, scaleY,
		anchorX - ((offsetX * scaleX) >> 8), anchorY - ((offsetY * scaleY) >> 8)
	};

	switch (reserves) {

		case EXTBGCTRL_MAIN_BG2:
		BACKGROUND.control[2] = controlBits;
		BACKGROUND.bg2_rotation = transform;
		break;

		case EXTBGCTRL_MAIN_BG3:
		BACKGROUND.control[3] = controlBits;
		BACKGROUND.bg3_rotation = transform;
		break;

		case EXTBGCTRL_SUB_BG2:
		BACKGROUND_SUB.control[2] = controlBits;
		BACKGROUND_SUB.bg2_rotation = transform;
		break;

		case EXTBGCTRL_SUB_BG3:
		BACKGROUND_SUB.control[3] = controlBits;
		BACKGROUND_SUB.bg3_rotation = transform;
		break;
	}
}

void ExtBgCtrl::setMapBase(u32 mapBase) {

	this->mapBase = mapBase;
}

void ExtBgCtrl::setTileBase(u32 tileBase) {

	this->tileBase = tileBase;
}

void ExtBgCtrl::setPriority(u32 priority) {

	this->priority = priority;
}

void ExtBgCtrl::setMosaic(bool mosaic) {

	this->mosaic = mosaic;
}

void ExtBgCtrl::setWrap(bool wrap) {

	this->wrap = wrap;
}

void ExtBgCtrl::setAnchor(s32 x, s32 y) {

	anchorX = x;
	anchorY = y;
}

void ExtBgCtrl::setOffset(s32 x, s32 y) {

	offsetX = x;
	offsetY = y;
}

void ExtBgCtrl::addOffset(s32 x, s32 y) {

	offsetX += x;
	offsetY += y;
}

void ExtBgCtrl::setScale(s16 x, s16 y) {

	scaleX = x;
	scaleY = y;
}

void ExtBgCtrl::setMatrix(s32 scaleX, s32 scaleY, s32 shearX, s32 shearY) {

	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->shearX = shearX;
	this->shearY = shearY;
}

u16 *ExtBgCtrl::mapData() {

	if (reserves == EXTBGCTRL_MAIN_BG2 || reserves == EXTBGCTRL_MAIN_BG3) {
		return BG_MAP_RAM(mapBase);
	}
	else {
		return BG_MAP_RAM_SUB(mapBase);
	}
}

u16 *ExtBgCtrl::tileData() {

	if (reserves == EXTBGCTRL_MAIN_BG2 || reserves == EXTBGCTRL_MAIN_BG3) {
		return BG_TILE_RAM(tileBase);
	}
	else {
		return BG_TILE_RAM_SUB(tileBase);
	}
}

u16 *ExtBgCtrl::basePalette() {

	if (reserves == EXTBGCTRL_MAIN_BG2 || reserves == EXTBGCTRL_MAIN_BG3) {
		return BG_PALETTE;
	}
	else {
		return BG_PALETTE_SUB;
	}
}
