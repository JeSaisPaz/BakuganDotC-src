// bdc 0x0894ee90 UiNetMenuCheckConfirm
#include "bdc.h"

/* Checks Cross (pad `pressed` 0x4000) in `UiNetMenu`: returns 0 when not pressed, 1
   when the chosen button `+0x74` is enabled, 2 when it is disabled. */

s32 UiNetMenuCheckConfirm(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;

  if ((menu->base.pad->pressed & 0x4000) == 0) {
    return 0;
  }
  if (menu->enabled[menu->choice] != 0) {
    return 1;
  }
  return 2;
}
