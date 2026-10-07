// bdc 0x089fdc54 IoDecodeCtor
#include "bdc.h"

/* Constructor of a decode job (`CODecode`, 0x10b0 bytes): a `CoreNode` with vtable
   `g_ioDecodeJobVtbl`, flags `done` (+0x24) and `cancelled` (+0x25) cleared, `streamed` (+0x26)
   = `streamed` and `waitingInput` (+0x27) = `streamed`; next input chunk `chunk` (+0x1064) = `src`,
   `outputSize` (+0x106c) = `CoreLzssGetSize``(src)`, output buffer `output` (+0x1068) = `dest`, or
   a fresh heap block of `outputSize` bytes when `dest` is 0/1 (1 = low heap end); starts the LZSS
   stream state at +0x28 (`CoreLzssStreamInit`) and creates the `"Mutex_CODecode"` lock
   (`lock` +0x1074). `arg` is unused. Returns `self`. */

IoDecodeJob *IoDecodeCtor(IoDecodeJob *self, void *src, void *arg, u8 *dest, bool streamed)
{
  bool fromLow;
  u32 size;
  CoreLock *lock;
  CoreLock *storage;

  CoreNodeCtor(&self->base, NULL);
  self->streamed = streamed;
  self->base.vtable = g_ioDecodeJobVtbl;
  self->done = 0;
  self->cancelled = 0;
  self->waitingInput = self->streamed;
  self->chunk = src;
  self->outputSize = CoreLzssGetSize(src);
  if (((uintptr_t)dest & ~(uintptr_t)1) == 0) {
    size = self->outputSize;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow((uintptr_t)dest == 1);
    dest = MemAlloc(size, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
  }
  self->output = dest;
  CoreLzssStreamInit(&self->stream, src, dest, 0);
  storage = &self->lockStorage;
  lock = NULL;
  if (storage != NULL) {
    CoreLockInit(storage, "Mutex_CODecode", CORE_LOCK_MUTEX);
    lock = storage;
  }
  self->lock = lock;
  return self;
}
