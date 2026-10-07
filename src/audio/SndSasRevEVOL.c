// bdc 0x08a292e0 SndSasRevEVOL
#include "bdc.h"

/* Sets the reverb return volume (`__sceSasRevEVOL`). Returns `0x80420100` when the SAS layer is not
   initialised (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `0x08b02c80`. Rejected
   with `0x80420024` in multichannel output mode. */

int SndSasRevEVOL(int left, int right)
{
  int mode;

  if (g_sndSasInitialized == 0) {
    return -0x7fbdff00;
  }
  mode = -0x7fbdff00;
  if (g_sndSasInitialized == 1) {
    mode = __sceSasGetOutputmode(&g_sndSasCore);
  }
  if (mode == 1) {
    return -0x7fbdffdc;
  }
  return __sceSasRevEVOL(&g_sndSasCore, left, right);
}
