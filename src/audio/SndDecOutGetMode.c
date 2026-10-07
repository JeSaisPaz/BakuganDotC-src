// bdc 0x089c4a80 SndDecOutGetMode
#include "bdc.h"

/* Returns the decoder's current output `mode` (+8) under its lock. */

s32 SndDecOutGetMode(SndDecOut *dec)

{
  s32 mode;
  
  CoreLockAcquire(dec->lock);
  mode = dec->mode;
  CoreLockRelease(dec->lock);
  return mode;
}

