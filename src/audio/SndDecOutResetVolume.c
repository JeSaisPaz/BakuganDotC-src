// bdc 0x089c4c5c SndDecOutResetVolume
#include "bdc.h"

/* Resets the decoder's fade and sample-read state under its lock: `volFrom = volTo = 0`, `fadeLeft
   = fadeTotal = 0`, `decodePos = decodeRemain = 0` (the next block triggers a fresh
   `sceAtracDecodeData`). */

void SndDecOutResetVolume(SndDecOut *dec)

{
  CoreLockAcquire(dec->lock);
  dec->volTo = 0;
  dec->volFrom = 0;
  dec->fadeTotal = 0;
  dec->fadeLeft = 0;
  dec->decodePos = 0;
  dec->decodeRemain = 0;
  CoreLockRelease(dec->lock);
  return;
}

