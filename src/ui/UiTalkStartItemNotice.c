// bdc 0x0882c2d0 UiTalkStartItemNotice
#include "bdc.h"

/* Starts the item-pickup notice on the talk/HUD task (`UiGetTalkTask`) (id 0x6e, `BtlHudUpdate`):
   unless `src` has its byte `+0x4c1` set, stores `src` in `+0x64` and sets `+0x4dc = 1`. Called by
   `BtlItemApplyPickup`. */

void UiTalkStartItemNotice(BtlHud *win, BtlBakugan *src)

{
  if (src->combat.dead == 0) {
    win->gateCutInState = 1;
    win->gateCutInUnit = src;
  }
  return;
}

