// bdc 0x089405e8 UiScreen390SelectTerminal
#include "bdc.h"

/* Maps the argument `stagePoint` of `UiScreen390` (1–3) to the terminal index
   `terminal` (0, 1, 2; left unchanged otherwise) and sets `phaseStep` to 0x14. */

void UiScreen390SelectTerminal(UiScreen *self)

{
  UiScreen390 *screen;
  s32 point;

  screen = (UiScreen390 *)self;
  point = screen->stagePoint;
  if (point < 2) {
    if (0 < point) {
      screen->terminal = 0;
      screen->base.phaseStep = 0x14;
      return;
    }
  }
  else {
    if (point < 3) {
      screen->terminal = 1;
      screen->base.phaseStep = 0x14;
      return;
    }
    if (point < 4) {
      screen->terminal = 2;
    }
  }
  screen->base.phaseStep = 0x14;
  return;
}
