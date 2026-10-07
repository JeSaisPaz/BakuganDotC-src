// bdc 0x08992078 UiUnlockCodeGetGlyphOffset
#include "bdc.h"

/* Returns the per-character drawing offset of keyboard character `ch` for
   `UiUnlockCode`: `*dy` = −2 and `*dx` = −2 adjusted per glyph:
   −3 for 0x52/0x58/0x62/0x67/0x8a, −6 for 0x55/0x7d, +1 for 0x5c/0x5d, −5 for 0x60/0x65,
   +2 for 0x84/0xbb; every other character keeps −2. */

void UiUnlockCodeGetGlyphOffset(int ch, float *dx, float *dy)
{
  *dy = -2.0f;
  *dx = -2.0f;
  switch (ch) {
  case 0x55:
  case 0x7d:
    *dx = *dx - 6.0f;
    return;
  case 0x62:
  case 0x8a:
    *dx = *dx - 3.0f;
    return;
  case 0x52:
  case 0x58:
  case 0x67:
    *dx = *dx - 3.0f;
    return;
  case 0x60:
  case 0x65:
    *dx = *dx - 5.0f;
    return;
  case 0x5c:
  case 0x5d:
    *dx = *dx + 1.0f;
    return;
  case 0x84:
    *dx = *dx + 2.0f;
    return;
  case 0xbb:
    *dx = *dx + 2.0f;
    return;
  }
}
