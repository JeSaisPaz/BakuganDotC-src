// bdc 0x0892c354 UiAttributeGetIconCell
#include "bdc.h"

/* Packs attribute `attr` with its icon cell from the 12-byte table `g_uiAttributeIconCells`: returns `attr |
   col << 8 | row << 16`. */

u32 UiAttributeGetIconCell(u8 attr)
{
  u8 table[12];
  u8 out[4];

  memcpy(table, g_uiAttributeIconCells, 12);
  memset(out, 0, 4);
  out[0] = attr;
  out[1] = table[attr * 2];
  out[2] = table[attr * 2 + 1];
  return *(u32 *)out;
}
