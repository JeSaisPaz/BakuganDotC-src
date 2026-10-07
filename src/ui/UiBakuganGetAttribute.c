// bdc 0x0892bbd8 UiBakuganGetAttribute
#include "bdc.h"

/* Returns the attribute index of Bakugan `id` from the 33-byte table `0x08ac13c8`. */

u8 UiBakuganGetAttribute(u32 id)

{
  u8 table[36];
  
  memcpy(table,g_bakuganAttributeTable,0x21);
  return table[id & 0xff];
}

