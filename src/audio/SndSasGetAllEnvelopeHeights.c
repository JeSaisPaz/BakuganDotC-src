// bdc 0x08a29210 SndSasGetAllEnvelopeHeights
#include "bdc.h"

/* Reads the current envelope height of all 32 SAS voices into `heights`
   (`__sceSasGetAllEnvelopeHeights`). Returns `0x80420100` when the SAS layer is not initialised
   (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `g_sndSasCore`. */

int SndSasGetAllEnvelopeHeights(int *heights)

{
  int ret;

  ret = 0x80420100;
  if (g_sndSasInitialized != 0) {
    ret = __sceSasGetAllEnvelopeHeights(&g_sndSasCore,heights);
  }
  return ret;
}

