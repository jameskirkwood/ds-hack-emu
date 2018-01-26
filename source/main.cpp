#include <nds.h>

#include "App.h"

int main() {

  defaultExceptionHandler();

  APP.init();

  while (true) {
    swiWaitForVBlank();
    APP.draw();
    APP.tick();
  }

  return 0;
}
