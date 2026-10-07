// bdc 0x08a29180 SndSasSetSimpleADSR
#include "bdc.h"

/* Sets the two packed ADSR envelope words of SAS voice `voice` (`__sceSasSetSimpleADSR`). Returns
   `0x80420100` when the SAS layer is not initialised (`g_sndSasInitialized == 0`). */

int SndSasSetSimpleADSR(int voice, u32 adsr1, u32 adsr2)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    ret = __sceSasSetSimpleADSR(&g_sndSasCore, voice, adsr1, adsr2);
  }
  return ret;
}
