// bdc 0x089c49d4 SndDecOutDestroyObj
#include "bdc.h"

/* Destructor of `SndDecOut`: marks the paired player (`SndBgmPlayerExists(channel)`) as
   `decoderQuiesced = 1`, frees the ring and the decode buffer (`MemFreeAligned`), destroys the
   mutex (`CoreLockDestroy`) and, when `flags & 1`, frees the object with `MemFree` under the
   heap lock. */

void SndDecOutDestroyObj(SndDecOut *dec, u32 flags)

{
  if (dec != (SndDecOut *)0x0) {
    if (SndBgmPlayerExists(dec->channel)) {
      SndBgmPlayerGet(dec->channel)->decoderQuiesced = 1;
    }
    MemFreeAligned(dec->ring);
    MemFreeAligned(dec->decodeBuf);
    if (dec->lock != (CoreLock *)0x0) {
      CoreLockDestroy(dec->lock,3);
      dec->lock = (CoreLock *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(dec,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

