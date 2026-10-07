// bdc 0x08a29024 SndSasSetVolume
#include "bdc.h"

/* Sets the dry and effect (reverb send) volumes of SAS voice `voice` (`__sceSasSetVolume`). Returns
   `0x80420100` when the SAS layer is not initialised (`g_sndSasInitialized == 0`); the `SceSasCore`
   work area is `g_sndSasCore`. */

int SndSasSetVolume(int voice, int left, int right, int effectLeft, int effectRight)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    ret = __sceSasSetVolume(&g_sndSasCore, voice, left, right, effectLeft, effectRight);
  }
  return ret;
}
