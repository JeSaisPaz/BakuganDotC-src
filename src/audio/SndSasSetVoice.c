// bdc 0x08a290d8 SndSasSetVoice
#include "bdc.h"

/* Points SAS voice `voice` at VAG (ADPCM) sample data (`__sceSasSetVoice(core, voice, vagAddr,
   size, loop)`). Returns `0x80420100` when the SAS layer is not initialised
   (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `g_sndSasCore`. */

int SndSasSetVoice(int voice, void *vagAddr, int size, int loop)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    ret = __sceSasSetVoice(&g_sndSasCore, voice, vagAddr, size, loop);
  }
  return ret;
}
