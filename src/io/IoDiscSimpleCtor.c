// bdc 0x089f9444 IoDiscSimpleCtor
#include "bdc.h"

/* Constructor of the `CODiscSimple` disc reader (the object behind `g_discSimple`): installs
   `g_discSimpleVtbl` at `vtbl`, resets the request fields (`state`, `mode`, `fd = -1`, the
   flags `streamed..destResolved` with `idle = destFromLow = 1`), creates the LwMutex `CoreLock`
   `"Mutex_CODiscSimple"` (`lock`, NULL when the low-heap allocation fails), and builds the two
   read-buffer nodes `bufNodes[0..1]`: `CoreNode`s with vtable `g_ioDiscBufNodeVtbl`, each
   owning a 0x30000-byte aligned buffer (NULL entry when the node allocation fails). Sets
   `medium = 1`, `msError = 0`, calls `IoDiscSetError(self, 0)` and clears the remaining flags.
   Returns `self`. */

IoDiscSimple *IoDiscSimpleCtor(IoDiscSimple *self)
{
  bool wasLow;
  CoreLock *lock;
  IoDiscBufNode *node;
  int i;

  self->vtbl = g_discSimpleVtbl;
  self->state = 0;
  self->mode = 0;
  self->streamed = 0;
  self->asyncIssued = 0;
  self->asyncPending = 0;
  self->busy = 0;
  self->idle = 1;
  self->destFromLow = 1;
  self->destResolved = 0;
  self->path = NULL;
  self->fd = -1;
  self->dest = NULL;
  self->size = 0;
  self->streamBytes = 0;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(sizeof(CoreLock), NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "Mutex_CODiscSimple", CORE_LOCK_LWMUTEX);
  }
  self->lock = lock;
  self->writeIndex = 0;
  self->readIndex = 0;

  for (i = 0; i < 2; i++) {
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    node = MemAlloc(sizeof(IoDiscBufNode), NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (node != NULL) {
      CoreNodeCtor(&node->base, NULL);
      node->base.vtable = g_ioDiscBufNodeVtbl;
      node->state = 0;
      node->data = MemAllocAligned(0x30000, true);
    }
    self->bufNodes[i] = node;
  }

  self->medium = 1;
  self->msError = 0;
  IoDiscSetError(self, 0);
  self->resumeRequested = 0;
  self->suspendAfterClose = 0;
  self->powerSuspend = 0;
  self->allocator = NULL;
  self->abortFlag = 0;
  self->dataLoaded = 0;
  return self;
}
