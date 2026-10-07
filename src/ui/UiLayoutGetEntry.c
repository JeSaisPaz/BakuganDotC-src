// bdc 0x0881a2c0 UiLayoutGetEntry
#include "bdc.h"

/* Returns entry `index` (0x14-byte records `{s16 x, y, z, u, v, w, h, …}`) of UI layout `layout`
   (0..63) from the table `g_uiLayoutTables`; an out-of-range index returns the first entry, an
   invalid layout NULL. */

s16 * UiLayoutGetEntry(s32 layout, s32 index)

{
  s16 *entry;

  entry = (s16 *)0x0;
  if ((-1 < layout) && ((u32)layout < 0x40)) {
    entry = g_uiLayoutTables[layout];
    if ((-1 < index) && (index < g_uiLayoutCounts[layout])) {
      entry = entry + index * 10;
    }
  }
  return entry;
}
