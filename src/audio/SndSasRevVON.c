// bdc 0x08a2924c SndSasRevVON
#include "bdc.h"

/* Sets the reverb dry/wet switches (`__sceSasRevVON`). Returns `0x80420100` when the SAS layer is
   not initialised (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `0x08b02c80`.
   Rejected with `0x80420024` when the core runs in multichannel output mode
   (`__sceSasGetOutputmode() == 1`). */

int SndSasRevVON(int dry, int wet)
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
  return __sceSasRevVON(&g_sndSasCore, dry, wet);
}
