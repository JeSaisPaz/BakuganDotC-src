// bdc 0x08a28f70 SndSasKeyOn
#include "bdc.h"

/* Keys on SAS voice `voice` (`__sceSasSetKeyOn`). Returns `0x80420100` when the SAS layer is not
   initialised (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `g_sndSasCore`. */

int SndSasKeyOn(int voice)

{
  int ret;

  ret = 0x80420100;
  if (g_sndSasInitialized != 0) {
    ret = __sceSasSetKeyOn(&g_sndSasCore,voice);
  }
  return ret;
}

