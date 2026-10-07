// bdc 0x0892c09c UiBakuganGetModelScale
#include "bdc.h"

/* Returns the menu display scale of Bakugan `id`, form `form` (table `g_uiBakuganModelScaleTable`,
   2 floats per id). */

float UiBakuganGetModelScale(u32 id, u32 form)

{
  float table[44];

  memcpy(table, g_uiBakuganModelScaleTable, 0xb0);
  return table[(id & 0xff) * 2 + (form & 0xff)];
}
