// bdc 0x0882c28c UiSetWindowActive
#include "bdc.h"

/* Stores `value` into byte `kind` of the 15-entry table at `g_uiWindowActive` (ignored when `kind` is
   outside 0..14); read back with `UiGetWindowActive`. */

void UiSetWindowActive(s32 kind, u8 value)

{
  if ((-1 < kind) && (kind < 0xf)) {
    g_uiWindowActive[kind] = value;
  }
  return;
}

