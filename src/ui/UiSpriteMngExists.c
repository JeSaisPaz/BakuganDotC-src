// bdc 0x089ee168 UiSpriteMngExists
#include "bdc.h"

/* Returns whether the sprite manager task (core id `0x276a`) exists (`CoreTaskExists`). */

bool UiSpriteMngExists(void)

{
  s32 exists;
  
  exists = CoreTaskExists(0x276a);
  return exists != 0;
}

