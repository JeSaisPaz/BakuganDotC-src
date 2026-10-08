// bdc 0x0881ae70 NetPlayCtor
#include "bdc.h"

/* Constructor of the 0xf0-byte `NetPlay` manager: creates its `"CONetPlay"`
   `CoreLock` (LwMutex, `lock`, NULL if the low-heap allocation fails), clears state, finished,
   abortRequested, peerCount, the 0x7c-byte peer table, hasSelectedHost, newFrame/synced, flags,
   exchangeState and publishSkipped, sets localSlot and maxFrameSeen to -1, creates a 0x50-byte
   `PadState` for the remote pad (`remotePad`, `PadBaseCtor` + `PadInit`, d-pad stick
   emulation off), clears updateRan/resetOnConnect/umdWaitFrames, resets the local player index
   (`NetSetLocalPlayerIndex``(-1)`) and returns `self`. */

NetPlay *NetPlayCtor(NetPlay *self)
{
  bool fromLow;
  CoreLock *lock;
  PadState *pad;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(sizeof(CoreLock), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "CONetPlay", CORE_LOCK_LWMUTEX);
  }
  self->lock = lock;
  self->state = 0;
  self->finished = 0;
  self->abortRequested = 0;
  self->peerCount = 0;
  self->hasSelectedHost = 0;
  memset(self->peers, 0, sizeof(self->peers));
  self->newFrame = 0;
  self->synced = 0;
  self->flags = 0;
  self->localSlot = -1;
  self->exchangeState = 0;
  self->publishSkipped = 0;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pad = MemAlloc(__builtin_offsetof(PadState, id50), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (pad != NULL) {
    PadBaseCtor(pad);
  }
  self->remotePad = pad;
  PadInit(pad, 0);
  self->remotePad->dpadEmulatesStick = 0;
  self->maxFrameSeen = -1;
  self->updateRan = 0;
  self->resetOnConnect = 0;
  NetSetLocalPlayerIndex(-1);
  self->umdWaitFrames = 0;
  return self;
}
