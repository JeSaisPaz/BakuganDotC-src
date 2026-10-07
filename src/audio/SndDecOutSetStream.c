// bdc 0x089c4b9c SndDecOutSetStream
#include "bdc.h"

/* Hands a prepared Atrac stream to the decoder: if `atracId >= 0` and `data` is non-NULL stores
   `atracId`, `dataBuffer`, `dataSize`, enables decoding (`SndDecOutSetDecodeEnabled``(dec, 1)`)
   and clears the `unke8` marker (-1); then wakes the decoder thread if it is sleeping. Returns 1 on
   success, 0 if the arguments were rejected. */

s32 SndDecOutSetStream(SndDecOut *dec, s32 atracId, void *data, u32 size)

{
  s32 ok;
  
  ok = 0;
  CoreLockAcquire(dec->lock);
  if ((-1 < atracId) && (data != (void *)0x0)) {
    dec->atracId = atracId;
    dec->dataBuffer = data;
    dec->dataSize = size;
    SndDecOutSetDecodeEnabled(dec,1);
    dec->unke8 = -1;
    ok = 1;
  }
  CoreLockRelease(dec->lock);
  if ((ok != 0) &&
     BootIsThreadSleeping(dec->channel + 6) != 0) {
    BootWakeupThread(dec->channel + 6);
  }
  return ok;
}

