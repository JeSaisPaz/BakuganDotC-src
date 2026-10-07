// bdc 0x0880cc14 SaveProfileWordMax
#include "bdc.h"

/* Upper bound of profile word `index` for `SaveProfileClampWord`: 0x20 for words 3–6, 999 for
   words 0xb–0x11, `INT_MAX` otherwise. */

s32 SaveProfileWordMax(SaveProfile *self, s32 index)

{
  switch(index) {
  case 3:
    return 0x20;
  case 4:
    return 0x20;
  case 5:
    return 0x20;
  case 6:
    return 0x20;
  case 7:
  case 8:
  case 9:
  case 10:
    break;
  case 0xb:
    return 999;
  case 0xc:
    return 999;
  case 0xd:
    return 999;
  case 0xe:
    return 999;
  case 0xf:
    return 999;
  case 0x10:
    return 999;
  case 0x11:
    return 999;
  }
  return 0x7fffffff;
}

