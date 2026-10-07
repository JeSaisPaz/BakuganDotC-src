// bdc 0x089c47d4 SndDecOutExists
#include "bdc.h"

/* Returns 1 when decoder channel `channel` (0..2) has a live `SndDecOut`; with `channel < 0`
   returns 1 if any of the three slots is used; 0 without a table or for an out-of-range channel. */

s32 SndDecOutExists(s32 channel)
{
  s32 i;

  if (g_soundDecOutTable == NULL) {
    return 0;
  }
  if (channel < 0) {
    for (i = 0; i < 3; i++) {
      if (g_soundDecOutTable->slots[i] != NULL) {
        return 1;
      }
    }
    return 0;
  }
  if (channel < 3 && g_soundDecOutTable->slots[channel] != NULL) {
    return 1;
  }
  return 0;
}
