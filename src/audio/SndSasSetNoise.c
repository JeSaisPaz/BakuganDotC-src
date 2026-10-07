// bdc 0x08a29134 SndSasSetNoise
#include "bdc.h"

/* Switches SAS voice `voice` to the noise generator at clock `freq` (`__sceSasSetNoise`). Returns
   `0x80420100` when the SAS layer is not initialised (`g_sndSasInitialized == 0`). */

int SndSasSetNoise(int voice, int freq)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    ret = __sceSasSetNoise(&g_sndSasCore, voice, freq);
  }
  return ret;
}
