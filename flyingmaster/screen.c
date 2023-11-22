#include "screen.h"
#include <stdlib.h>

void screenInit() {
  screenClear();
  screenHideCursor();
}

void screenDestroy() {
  screenShowCursor();
  screenClear();
}
