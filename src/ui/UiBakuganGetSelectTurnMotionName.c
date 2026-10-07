// bdc 0x0892c7d8 UiBakuganGetSelectTurnMotionName
#include "bdc.h"

/* Copies the select motion name of Bakugan `id` from the second 22-entry table `0x08ac193c`
   (`*_stay_sel`, `*_stay_sel_turn`, ...) into the 64-byte buffer `out`. */

void UiBakuganGetSelectTurnMotionName(u32 id, char *out)

{
  const char *names[22];
  char buf[64];
  u32 i;

  memcpy(names,g_uiBakuganSelectTurnMotionNames,sizeof(names));
  memset(buf,0,sizeof(buf));
  sprintf(buf,names[id & 0xff]);
  for (i = 0; i < 0x40; i++) {
    out[i] = buf[i];
  }
}
