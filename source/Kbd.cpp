#include <nds.h>

#include "Kbd.h"

Kbd KBD;

void Kbd::init() {

	;
}

void Kbd::tick() {

	scanKeys();
}

u16 Kbd::read() {

	if (keysHeld() & KEY_LEFT)  return 130;
	if (keysHeld() & KEY_RIGHT) return 132;

	return 0;
}
