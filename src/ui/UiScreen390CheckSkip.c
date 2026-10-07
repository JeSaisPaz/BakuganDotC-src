// bdc 0x08940794 UiScreen390CheckSkip
#include "bdc.h"

/* Sets the fast-forward flag `g_uiScreen390FastForward` once Cross is pressed (pad `pressed` 0x4000) while it is
   still clear. */

void UiScreen390CheckSkip(UiScreen *screen)

{
  if ((g_uiScreen390FastForward == 0) && ((screen->pad->pressed & 0x4000) != 0)) {
    g_uiScreen390FastForward = 1;
  }
  return;
}

