// bdc 0x0882c1e0 UiGetWindowActive
#include "bdc.h"

/* Returns byte `kind` of `g_uiWindowActive` (0 when `kind` is outside 0..14): the
   "window/overlay kind is active" flags. */

u8 UiGetWindowActive(s32 kind)
{
  if (kind >= 0 && kind < 0xf) {
    return g_uiWindowActive[kind];
  }
  return 0;
}
