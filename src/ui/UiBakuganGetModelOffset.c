// bdc 0x0892c3b8 UiBakuganGetModelOffset
#include "bdc.h"

/* Returns entry `i` (0..3) of the menu placement values of Bakugan `id` (table
   `g_uiBakuganModelOffsetTable`, 4 floats per id). */

float UiBakuganGetModelOffset(u32 id, u32 i)

{
  float table[84];

  memcpy(table, g_uiBakuganModelOffsetTable, 0x150);
  return table[(id & 0xff) * 4 + (i & 0xff)];
}
