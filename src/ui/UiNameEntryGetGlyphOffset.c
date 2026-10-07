// bdc 0x08804664 UiNameEntryGetGlyphOffset
#include "bdc.h"

/* Returns per-character drawing offsets for character index `ch` of the name-entry character table:
   `*dy` = -2.0 and `*dx` = -2.0 adjusted for a few narrow/wide glyphs (0x52/0x58/0x62/0x67/0x8a -3,
   0x55/0x7d -6, 0x5c/0x5d +1, 0x60/0x65 -5, 0x84/0xbb +2). Used by `UiNameEntryRedrawName` and
   `UiNameEntryRedrawKeyGrid` of `UiNameEntry`. */

void UiNameEntryGetGlyphOffset(s32 ch, float *dx, float *dy)

{
  *dy = -2.0f;
  *dx = -2.0f;
  switch (ch) {
  case 0x55:
  case 0x7d:
    *dx = *dx - 6.0f;
    break;
  case 0x52:
  case 0x58:
  case 0x62:
  case 0x67:
  case 0x8a:
    *dx = *dx - 3.0f;
    break;
  case 0x60:
  case 0x65:
    *dx = *dx - 5.0f;
    break;
  case 0x5c:
  case 0x5d:
    *dx = *dx + 1.0f;
    break;
  case 0x84:
  case 0xbb:
    *dx = *dx + 2.0f;
    break;
  }
}
