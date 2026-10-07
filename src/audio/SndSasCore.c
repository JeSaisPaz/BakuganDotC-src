// bdc 0x08a28db4 SndSasCore
#include "bdc.h"

/* Renders one SAS grain into `out` with `__sceSasCore`. Returns `0x80420100` when the SAS layer is
   not initialised (`g_sndSasInitialized == 0`); the `SceSasCore` work area is `g_sndSasCore`. `out`
   must be non-NULL and 64-byte aligned, else `0x80420005`. */

int SndSasCore(void *out)
{
  int ret = (int)0x80420100;

  if (g_sndSasInitialized != 0) {
    ret = (int)0x80420005;
    if (out != NULL && ((uintptr_t)out & 0x3f) == 0) {
      ret = __sceSasCore(&g_sndSasCore, out);
    }
  }
  return ret;
}
