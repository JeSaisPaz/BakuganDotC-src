// bdc 0x08a29374 SndSasRevType
#include "bdc.h"

/* Selects the reverb effect type (`__sceSasRevType`). Returns `0x80420100` when the SAS layer is
   not initialised (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `g_sndSasCore`.
   Rejected with `0x80420024` in multichannel output mode. */

int SndSasRevType(int type)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    int mode = ret;
    if (g_sndSasInitialized == 1) {
      mode = __sceSasGetOutputmode(&g_sndSasCore);
    }
    ret = (int)0x80420024;
    if (mode != 1) {
      ret = __sceSasRevType(&g_sndSasCore, type);
    }
  }
  return ret;
}
