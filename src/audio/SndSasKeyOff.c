// bdc 0x08a28fac SndSasKeyOff
#include "bdc.h"

/* Keys off SAS voice `voice` (`__sceSasSetKeyOff`). Returns `0x80420100` when the SAS layer is not
   initialised (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `g_sndSasCore`. */

int SndSasKeyOff(int voice)

{
  int ret;

  ret = 0x80420100;
  if (g_sndSasInitialized != 0) {
    ret = __sceSasSetKeyOff(&g_sndSasCore,voice);
  }
  return ret;
}

