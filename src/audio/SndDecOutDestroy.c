// bdc 0x089c46f4 SndDecOutDestroy
#include "bdc.h"

/* Destroys the decoder channel `channel` (0..2), or all of them when `channel < 0`: each live slot
   is destructed with `SndDecOutDestroyObj``(dec, 3)`, cleared, the table count decremented
   (zeroed for "all"), and the decoder thread (slot channel + 6) is stopped with
   `BootDeleteThread`. */

void SndDecOutDestroy(s32 channel)

{
  SndDecOutTable *table;
  s32 i;

  if (g_soundDecOutTable != (SndDecOutTable *)0x0) {
    if (channel < 0) {
      i = 0;
      do {
        if (g_soundDecOutTable->slots[i] != (SndDecOut *)0x0) {
          SndDecOutDestroyObj(g_soundDecOutTable->slots[i], 3);
          g_soundDecOutTable->slots[i] = (SndDecOut *)0x0;
        }
        BootDeleteThread(i + 6);
        i = i + 1;
      } while (i < 3);
      g_soundDecOutTable->count = 0;
    }
    else if ((channel < 3) && (g_soundDecOutTable->slots[channel] != (SndDecOut *)0x0)) {
      SndDecOutDestroyObj(g_soundDecOutTable->slots[channel], 3);
      table = g_soundDecOutTable;
      g_soundDecOutTable->slots[channel] = (SndDecOut *)0x0;
      table->count = table->count + -1;
      BootDeleteThread(channel + 6);
    }
  }
  return;
}
