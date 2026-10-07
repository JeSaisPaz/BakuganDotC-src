// bdc 0x089c4854 SndDecOutInit
#include "bdc.h"

/* Constructor of `SndDecOut` (`"COSoundDecOut"`): creates its 0x38-byte mutex (heap bottom,
   `CoreLockInit` type `CORE_LOCK_MUTEX`), sets `mode` and `requestedMode` to `mode`, `channel`
   and `atracId` to -1, `baseVolume = 1.0`, allocates the two-block output ring (2 × the manager's
   0x400-byte block) and the 0x2000-byte decode buffer with `MemAllocAligned` (64-byte aligned,
   from the heap bottom, zeroed), clears the other fields and calls `SndDecOutResetVolume`.
   Returns `dec`. */

SndDecOut *SndDecOutInit(SndDecOut *dec, s32 mode)
{
  bool fromLow;
  CoreLock *lock;
  s16 *ring;
  s16 *decodeBuf;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(sizeof(CoreLock), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "COSoundDecOut", CORE_LOCK_MUTEX);
  }
  dec->lock = lock;
  dec->pauseRequested = 0;
  dec->requestedMode = mode;
  dec->mode = mode;
  dec->dataBuffer = NULL;
  dec->dataSize = 0;
  ring = MemAllocAligned(SndGetManager()->blockBytes * 2 /* PSP: two blocks of blockBytes bytes */, true);
  dec->ring = ring;
  memset(ring, 0, SndGetManager()->blockBytes * 2 /* PSP: two blocks of blockBytes bytes */);
  decodeBuf = MemAllocAligned(0x1000 * sizeof(s16), true);
  dec->decodeBuf = decodeBuf;
  memset(decodeBuf, 0, 0x1000 * sizeof(s16));
  dec->readBlock = 0;
  dec->blockCount = 0;
  dec->decodePos = 0;
  dec->decodeRemain = 0;
  dec->finished = 0;
  dec->atracId = -1;
  dec->unk3c = 0;
  dec->paused = 0;
  dec->resumeRequested = 0;
  dec->channel = -1;
  dec->baseVolume = 1.0f;
  SndDecOutResetVolume(dec);
  memset(dec->unk54, 0, 0x80);
  dec->unkd4 = 0;
  dec->unkd8 = 0;
  dec->unkdc = 0;
  dec->unkdd = 0;
  dec->decodeEnabled = 0;
  dec->useOutput2 = 0;
  dec->unke8 = -1;
  return dec;
}
