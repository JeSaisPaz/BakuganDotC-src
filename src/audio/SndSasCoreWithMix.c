// bdc 0x08a28e0c SndSasCoreWithMix
#include "bdc.h"

/* Renders one SAS grain mixed into the existing samples of `inout` with `__sceSasCoreWithMix`.
   Returns `0x80420100` when the SAS layer is not initialised (`g_sndSasInitialized == 0`); the
   `SceSasCore` work area is `g_sndSasCore`. `inout` must be non-NULL and 64-byte aligned, else
   `0x80420005`. */

int SndSasCoreWithMix(void *inout, int leftVol, int rightVol)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    ret = (int)0x80420005;
    if (inout != NULL && ((uintptr_t)inout & 0x3f) == 0) {
      ret = __sceSasCoreWithMix(&g_sndSasCore, inout, leftVol, rightVol);
    }
  }
  return ret;
}
