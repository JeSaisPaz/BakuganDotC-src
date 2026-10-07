// bdc 0x0892bcc0 UiBakuganGetSelectMotionName
#include "bdc.h"

/* Copies the `*_stay_sel` idle motion name of Bakugan `id` (22-entry table `0x08ac15e4`:
   `00_dor_stay_sel`, `00_mdor_stay_sel`, ...) into the 64-byte buffer `out`. */

void UiBakuganGetSelectMotionName(u32 id, char *out)

{
  const char *names[22];
  char buf[64];
  u32 i;

  memcpy(names,g_uiBakuganSelectMotionNames,sizeof(names));
  memset(buf,0,sizeof(buf));
  sprintf(buf,names[id & 0xff]);
  for (i = 0; i < 0x40; i++) {
    out[i] = buf[i];
  }
}
