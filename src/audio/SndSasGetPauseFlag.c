// bdc 0x08a28fe8 SndSasGetPauseFlag
#include "bdc.h"

/* Returns the SAS pause-flag bit mask (`__sceSasGetPauseFlag`, bit n = voice n paused), or 0 when
   the SAS layer is not initialised. */

u32 SndSasGetPauseFlag(void)

{
  u32 ret;

  ret = 0;
  if (g_sndSasInitialized != 0) {
    ret = __sceSasGetPauseFlag(&g_sndSasCore);
  }
  return ret;
}

