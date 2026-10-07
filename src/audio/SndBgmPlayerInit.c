// bdc 0x089c3060 SndBgmPlayerInit
#include "bdc.h"

/* Constructor of `SndBgmPlayer` (the `"COSoundBGM"` streaming player): allocates its 0x38-byte
   mutex from the heap bottom (`MemAlloc` between `MemSetAllocFromLow` calls) and creates it with
   `CoreLockInit` (type `CORE_LOCK_MUTEX`; `lock` stays NULL if the allocation failed), clears
   the request/state fields, sets `atracId`, `trackId` and `prevTrackId` to -1, zeroes the
   0x80-byte path buffer and calls `SndBgmPlayerSetBuffer``(player, 1)` (default: let CODataMng
   allocate the file image from the heap bottom). Returns the player. */
SndBgmPlayer *SndBgmPlayerInit(SndBgmPlayer *player)
{
    bool fromLow;
    CoreLock *lock;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    lock = MemAlloc(sizeof(CoreLock), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (lock != NULL) {
        CoreLockInit(lock, "COSoundBGM", CORE_LOCK_MUTEX);
    }
    player->lock = lock;
    player->fileHandle = 0;
    player->buffer = (void *)(uintptr_t)1; /* "allocate from heap bottom" marker */
    player->loadFromLow = 1;
    player->externalBuffer = 0;
    player->unk16 = 0;
    player->loaded = 0;
    player->command = 0;
    player->state = 0;
    player->atracId = -1;
    player->trackId = -1;
    player->prevTrackId = -1;
    player->loopActive = 0;
    player->loopRequested = 0;
    memset(player->path, 0, sizeof(player->path));
    SndBgmPlayerSetBuffer(player, (void *)(uintptr_t)1);
    player->stopped = 1;
    player->resumePrevious = 0;
    player->bufferVolatile = 0;
    player->bufferReleased = 1;
    player->suspendRequested = 0;
    player->suspended = 0;
    player->decoderQuiesced = 1;
    player->resumeRequested = 0;
    player->volumePending = 0;
    player->decoderStop = 0;
    player->pendingVolume = 0;
    player->pendingFadeMs = 0;
    return player;
}
