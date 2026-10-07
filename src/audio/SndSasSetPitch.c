// bdc 0x08a29088 SndSasSetPitch
#include "bdc.h"

/* Sets the pitch of SAS voice `voice` (`__sceSasSetPitch`, `0x1000` = original rate). Returns
   `0x80420100` when the SAS layer is not initialised (`g_sndSasInitialized == 0`). A pitch of 0 is
   rejected with `0x80420012`. */

int SndSasSetPitch(int voice, int pitch)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    ret = (int)0x80420012;
    if (pitch != 0) {
      ret = __sceSasSetPitch(&g_sndSasCore, voice, pitch);
    }
  }
  return ret;
}
