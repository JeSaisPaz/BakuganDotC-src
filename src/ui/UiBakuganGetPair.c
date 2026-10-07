// bdc 0x0892adf4 UiBakuganGetPair
#include "bdc.h"

/* Returns entry `which` (0 = pair partner) of Bakugan `id` from the 21 x 2-byte pair table
   `g_uiBakuganPairTable`. */

u8 UiBakuganGetPair(u32 which, u32 id)

{
  u8 table[44];

  memcpy(table, g_uiBakuganPairTable, 0x2a);
  return table[(id & 0xff) * 2 + (which & 0xff)];
}
