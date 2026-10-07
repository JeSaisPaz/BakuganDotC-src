// bdc 0x089c4ab8 SndDecOutSetMode
#include "bdc.h"

/* Requests output mode `mode` (stored in `requestedMode`, which the decoder thread copies into
   `mode` once per block). When `g_soundDecOutAltMode` is set, modes 0 and 1 are shifted to 3 and
   4. */

void SndDecOutSetMode(SndDecOut *dec, s32 mode)
{
  CoreLockAcquire(dec->lock);
  if (g_soundDecOutAltMode != 0 && mode < 2) {
    mode = mode + 3;
  }
  dec->requestedMode = mode;
  CoreLockRelease(dec->lock);
}
