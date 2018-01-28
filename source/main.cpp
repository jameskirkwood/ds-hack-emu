#include <nds.h>

#include "App.h"

void vblank() {

    APP.draw();
    APP.tick();
}

int main() {

  defaultExceptionHandler();
  irqSet(IRQ_VBLANK, vblank);
  irqEnable(IRQ_VBLANK);

  APP.init();

  while (true) {
    APP.run();
    swiWaitForVBlank();
  }

  return 0;
}
