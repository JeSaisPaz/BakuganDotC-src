// bdc 0x08a291d4 SndSasGetEndFlag
#include "bdc.h"

/* Returns the SAS end-flag bit mask (`__sceSasGetEndFlag`, bit n = voice n finished), or 0 when the
   SAS layer is not initialised. */

u32 SndSasGetEndFlag(void)

{
  u32 ret;

  ret = 0;
  if (g_sndSasInitialized != 0) {
    ret = __sceSasGetEndFlag(&g_sndSasCore);
  }
  return ret;
}

