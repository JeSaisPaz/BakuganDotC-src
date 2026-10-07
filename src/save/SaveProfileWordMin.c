// bdc 0x0880cca4 SaveProfileWordMin
#include "bdc.h"

/* Lower bound of profile word `index` for `SaveProfileClampWord`: -1 for words 2–6, 0xb and
   0x19 (so they may hold the 'none' value -1), `INT_MIN` otherwise. */

s32 SaveProfileWordMin(SaveProfile *self, s32 index)

{
  s32 min;
  
  min = -0x80000000;
  switch(index) {
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 0xb:
  case 0x19:
    min = -1;
  }
  return min;
}

