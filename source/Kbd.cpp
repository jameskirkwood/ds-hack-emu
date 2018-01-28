#include <nds.h>

#include "Kbd.h"

Kbd KBD;

u16 Kbd::read() {

	if (keysHeld() & KEY_A)     return 128;
	if (keysHeld() & KEY_B)     return 129;
	if (keysHeld() & KEY_LEFT)  return 130;
	if (keysHeld() & KEY_UP)    return 131;
	if (keysHeld() & KEY_RIGHT) return 132;
	if (keysHeld() & KEY_DOWN)  return 133;

	return 0;
}
