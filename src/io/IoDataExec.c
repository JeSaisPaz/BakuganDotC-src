// bdc 0x089fbf70 IoDataExec
#include "bdc.h"

/* `COData::Exec()`: one step of a data request (`COData`) of the data manager, called from
   `IoDataMngUpdate`. Dispatches on the request state (`IoDataGetState`):
   - 0/1 (idle): flag 0x100 → state 9; flag 2 (load) → whole-file read into `buffer` (state 4) or,
     with flag 8 too (mask 10), a streamed read into `streamBuffer` (state 8), none while both
     0x80000010 bits are set; flag 1 → probe (state 2); flag 0x40 → info (state 3); flag 4 → streamed
     read appended into `buffer` (state 5);
   - 2/3: when the disc reader is idle, finishes the probe / stores size and start sector, back to
     state 0 and marks the request `done`;
   - 4: whole-file read finished → keeps the buffer (state 9) or, with flag 8, starts an LZSS decode
     job (state 6);
   - 5: copies each filled stream chunk into `buffer` (allocated here when it is NULL/1);
   - 6: decode job done → takes its output (state 9);
   - 8: feeds each stream chunk to a streamed decode job, created on the first chunk when the
     unpacked size (`CoreLzssGetSize`) fits into free memory; else prints
     " --- COData::Exec() --- 展開サイズが空き容量以上 --- %d / %d" ("unpacked size exceeds free
     memory") once per chunk (`g_ioDataLastOversizeSrc`) and retries next step;
   - 9: marks the request `done`.
   States 4, 5 and 8 back off while the reader is suspended (`IoDiscIsSuspended`).
   Returns 0 when the request reached `done` in this step (states 2/3 finished, state 9), else 1. */

u32 IoDataExec(IoData *self)
{
  u32 busy = 1;
  bool pending;
  bool ok;
  bool fromLow;
  bool prevFromLow;
  IoDiscBufNode *node;
  void *dest;
  u8 *buf;
  const u8 *src;
  u32 size;
  u32 len;
  u32 need;

  switch (IoDataGetState(self)) {
  case 0:
  case 1:
    if (IoDataHasFlags(self, 0x100)) {
      IoDataSetState(self, 9);
      IoDataClearFlags(self, 0x100);
    }
    else if (IoDataHasFlags(self, 2)) {
      if (!IoDiscHasManager()) {
        break;
      }
      if (!IoDataHasFlags(self, 0x80000010)) {
        if (IoDataHasFlags(self, 10)) {
          if (IoDiscRequestRead(IoDiscGetManager(), self->path, self->streamBuffer, true) != 0) {
            CoreLockAcquire(self->lock);
            self->loadedSize = -1;
            CoreLockRelease(self->lock);
            if (IoDataHasFlags(self, 0x20)) {
              IoDiscSetErrorFlags(IoDiscGetManager(), 1);
            }
            IoDiscSetAllocator(IoDiscGetManager(), self->allocator);
            if (IoDataHasFlags(self, 0x80)) {
              IoDiscSetFlagF4(IoDiscGetManager(), 1);
            }
            IoDataSetState(self, 8);
          }
        }
        else {
          if (IoDiscRequestRead(IoDiscGetManager(), self->path, self->buffer, false) != 0) {
            CoreLockAcquire(self->lock);
            self->loadedSize = -1;
            CoreLockRelease(self->lock);
            if (IoDataHasFlags(self, 0x20)) {
              IoDiscSetErrorFlags(IoDiscGetManager(), 1);
            }
            IoDiscSetAllocator(IoDiscGetManager(), self->allocator);
            if (IoDataHasFlags(self, 0x80)) {
              IoDiscSetFlagF4(IoDiscGetManager(), 1);
            }
            IoDataSetState(self, 4);
          }
        }
      }
      if (IoDataHasFlags(self, 1)) {
        IoDataClearFlags(self, 1);
      }
    }
    else if (IoDataHasFlags(self, 1)) {
      if (IoDiscRequestProbe(IoDiscGetManager(), self->path) != 0) {
        IoDataSetState(self, 2);
      }
    }
    else if (IoDataHasFlags(self, 0x40)) {
      if (IoDiscRequestInfo(IoDiscGetManager(), self->path) != 0) {
        IoDataSetState(self, 3);
      }
    }
    else if (IoDataHasFlags(self, 4)) {
      if (IoDiscRequestRead(IoDiscGetManager(), self->path, self->streamBuffer, true) != 0) {
        if (IoDataHasFlags(self, 0x20)) {
          IoDiscSetErrorFlags(IoDiscGetManager(), 1);
        }
        IoDiscSetAllocator(IoDiscGetManager(), self->allocator);
        IoDataSetState(self, 5);
        self->bufferSize = 0;
      }
    }
    break;

  case 2:
    if (IoDiscIsIdle(IoDiscGetManager())) {
      IoDiscFinishRequest(IoDiscGetManager());
      IoDataClearFlags(self, 1);
      IoDataSetState(self, 0);
      self->done = 1;
      busy = 0;
    }
    break;

  case 3:
    if (IoDiscIsIdle(IoDiscGetManager())) {
      self->streamBufferSize = IoDiscGetSize(IoDiscGetManager());
      self->bufferSize = IoDiscGetLba(IoDiscGetManager());
      IoDiscFinishRequest(IoDiscGetManager());
      IoDataClearFlags(self, 0x40);
      IoDataSetState(self, 0);
      self->done = 1;
      busy = 0;
    }
    break;

  case 4:
    if (IoDiscIsSuspended(IoDiscGetManager(), 0) != 0) {
      IoDiscTryResume(IoDiscGetManager());
      if (IoDiscIsSuspended(IoDiscGetManager(), 1) == 0) {
        IoDiscRequestResume(IoDiscGetManager());
      }
      break;
    }
    if (!IoDiscIsIdle(IoDiscGetManager())) {
      break;
    }
    dest = IoDiscGetDest(IoDiscGetManager());
    size = IoDiscGetSize(IoDiscGetManager());
    self->loadedSize = IoDiscGetOpenResult(IoDiscGetManager());
    IoDiscFinishRequest(IoDiscGetManager());
    IoDataClearFlags(self, 2);
    if (IoDataHasFlags(self, 0x20)) {
      IoDataClearFlags(self, 0x20);
    }
    if (self->loadedSize == 0) {
      CoreLockAcquire(self->lock);
      IoDataSetState(self, 9);
      CoreLockRelease(self->lock);
    }
    else if (IoDataHasFlags(self, 8)) {
      CoreLockAcquire(self->lock);
      self->streamBuffer = dest;
      self->streamBufferSize = size;
      self->decodeJob = IoDecodeMngNewJob(IoGetDecodeMng(), dest, (void *)(uintptr_t)size, NULL, false);
      CoreLockRelease(self->lock);
      CoreNodeOwnerAppend(&IoGetDecodeMng()->base, self->decodeJob);
      BootWakeupThread(3);
      IoDataSetState(self, 6);
    }
    else {
      CoreLockAcquire(self->lock);
      self->buffer = dest;
      self->bufferSize = size;
      CoreLockRelease(self->lock);
      IoDataSetState(self, 9);
    }
    CoreLockAcquire(self->lock);
    self->statusFlags |= 2;
    CoreLockRelease(self->lock);
    break;

  case 5:
    if (IoDiscIsSuspended(IoDiscGetManager(), 0) != 0) {
      self->bufferSize = 0;
      IoDiscStreamRelease(IoDiscGetManager(), -1);
      IoDiscTryResume(IoDiscGetManager());
      if (IoDiscIsSuspended(IoDiscGetManager(), 1) == 0) {
        IoDiscRequestResume(IoDiscGetManager());
      }
      break;
    }
    pending = true;
    if (IoDataHasFlags(self, 0x80000000)) {
      /* discard mode: drain the stream without copying */
      if (IoDiscIsIdle(IoDiscGetManager())) {
        pending = false;
      }
      else if (IoDiscIsWaitingForBuffer(IoDiscGetManager())) {
        IoDiscStreamResume(IoDiscGetManager());
      }
      if (IoDiscStreamPeek(IoDiscGetManager()) != NULL) {
        IoDiscStreamRelease(IoDiscGetManager(), 1);
        pending = true;
      }
      if (!pending) {
        IoDiscFinishRequest(IoDiscGetManager());
        IoDataClearFlags(self, 4);
        IoDataClearFlags(self, 0x80000000);
        IoDataSetState(self, 0);
      }
      break;
    }
    size = IoDiscGetOpenResult(IoDiscGetManager());
    self->loadedSize = size;
    if (size == 0) {
      if (IoDataHasFlags(self, 0x20)) {
        IoDataClearFlags(self, 0x20);
      }
      IoDiscFinishRequest(IoDiscGetManager());
      CoreLockAcquire(self->lock);
      IoDataSetState(self, 9);
      IoDataClearFlags(self, 4);
      self->statusFlags |= 4;
      CoreLockRelease(self->lock);
      break;
    }
    if (IoDiscIsIdle(IoDiscGetManager())) {
      if (IoDataHasFlags(self, 0x20)) {
        IoDataClearFlags(self, 0x20);
      }
      pending = false;
    }
    else if (IoDiscIsWaitingForBuffer(IoDiscGetManager())) {
      IoDiscStreamResume(IoDiscGetManager());
    }
    node = IoDiscStreamPeek(IoDiscGetManager());
    if (node != NULL) {
      CoreLockAcquire(self->lock);
      buf = self->buffer;
      if (buf == NULL || (uintptr_t)buf == 1) {
        /* no destination yet: allocate the whole file, from the low heap end when buffer == 1 */
        size = IoDiscGetSize(IoDiscGetManager());
        fromLow = self->buffer != NULL;
        MemLock();
        prevFromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(fromLow);
        buf = MemAlloc(size, NULL, 0);
        MemSetAllocFromLow(prevFromLow);
        MemUnlock();
        self->buffer = buf;
        self->ownsBuffer = 1;
      }
      buf += self->bufferSize;
      src = node->data;
      len = node->len;
      ok = true;
      if ((s32)len > 0) {
        if (CorePowerSafeMemcpy(CorePowerGet(), buf, src, len, self->lock) == 0) {
          ok = false;
          len = 0;
        }
      }
      self->bufferSize += len;
      CoreLockRelease(self->lock);
      pending = true;
      if (ok) {
        IoDiscStreamRelease(IoDiscGetManager(), 1);
      }
    }
    if (!pending) {
      IoDiscFinishRequest(IoDiscGetManager());
      IoDataClearFlags(self, 4);
      IoDataSetState(self, 9);
      self->statusFlags |= 4;
    }
    break;

  case 6:
    if (IoDecodeIsDone(self->decodeJob)) {
      dest = IoDecodeGetOutput(self->decodeJob);
      size = IoDecodeGetOutputSize(self->decodeJob);
      CoreLockAcquire(self->lock);
      IoDecodeCancel(self->decodeJob);
      self->decodeJob = NULL;
      self->buffer = dest;
      self->bufferSize = size;
      CoreLockRelease(self->lock);
      IoDataClearFlags(self, 8);
      IoDataSetState(self, 9);
      CoreLockAcquire(self->lock);
      self->statusFlags |= 8;
      CoreLockRelease(self->lock);
    }
    break;

  case 8:
    if (IoDiscIsSuspended(IoDiscGetManager(), 0) != 0) {
      if (self->decodeJob != NULL) {
        /* reader suspended mid-decode: drop the partial output */
        CoreLockAcquire(self->lock);
        if (IoDecodeIsWaitingInput(self->decodeJob) || IoDecodeIsDone(self->decodeJob)) {
          self->buffer = IoDecodeGetOutput(self->decodeJob);
          self->streamBuffer = NULL;
          self->streamBufferSize = 0;
          IoDecodeCancel(self->decodeJob);
          self->decodeJob = NULL;
          if (self->buffer != NULL) {
            dest = self->buffer;
            MemLock();
            MemFree(dest, NULL, 0);
            MemUnlock();
            self->buffer = NULL;
          }
          IoDiscStreamRelease(IoDiscGetManager(), 1);
        }
        CoreLockRelease(self->lock);
      }
      else {
        CoreLockAcquire(self->lock);
        self->bufferSize = 0;
        CoreLockRelease(self->lock);
        IoDiscStreamRelease(IoDiscGetManager(), -1);
        IoDiscTryResume(IoDiscGetManager());
        if (IoDiscIsSuspended(IoDiscGetManager(), 1) == 0) {
          IoDiscRequestResume(IoDiscGetManager());
        }
      }
      break;
    }
    pending = true;
    if (IoDiscIsIdle(IoDiscGetManager())) {
      pending = false;
    }
    else if (IoDiscIsWaitingForBuffer(IoDiscGetManager())) {
      IoDiscStreamResume(IoDiscGetManager());
    }
    node = IoDiscStreamPeek(IoDiscGetManager());
    if (self->decodeJob == NULL) {
      if (node != NULL) {
        /* first chunk: start the streamed decode when the unpacked data fits */
        CoreLockAcquire(self->lock);
        ok = true;
        self->streamBuffer = node->data;
        self->streamBufferSize = node->len;
        self->bufferSize = CoreLzssGetSize(self->streamBuffer);
        if (MemGetFreeSize() < self->bufferSize) {
          ok = false;
          if (g_ioDataLastOversizeSrc != self->streamBuffer) {
            g_ioDataLastOversizeSrc = self->streamBuffer;
            need = self->bufferSize;
            printf(" --- COData::Exec() --- 展開サイズが空き容量以上 --- %d / %d\n", need,
                   MemGetFreeSize());
            printf(" ==> <%s>\n", IoDataGetPath(self));
          }
        }
        if (ok) {
          self->decodeJob = IoDecodeMngNewJob(IoGetDecodeMng(), self->streamBuffer,
                                              (void *)(uintptr_t)self->bufferSize, self->buffer,
                                              true);
          CoreLockRelease(self->lock);
          CoreNodeOwnerAppend(&IoGetDecodeMng()->base, self->decodeJob);
          IoDecodeFeed(self->decodeJob, self->streamBufferSize, self->streamBuffer);
        }
        else {
          CoreLockRelease(self->lock);
        }
        pending = true;
      }
    }
    else if (IoDecodeIsDone(self->decodeJob)) {
      self->streamBuffer = NULL;
      self->streamBufferSize = 0;
      IoDiscStreamRelease(IoDiscGetManager(), 1);
    }
    else {
      pending = true;
      if (IoDecodeIsWaitingInput(self->decodeJob) && node != NULL) {
        if (self->streamBuffer == node->data) {
          /* the job consumed the current chunk: release it and look at the next one */
          self->streamBuffer = NULL;
          self->streamBufferSize = 0;
          IoDiscStreamRelease(IoDiscGetManager(), 1);
          node = IoDiscStreamPeek(IoDiscGetManager());
        }
        if (node != NULL) {
          CoreLockAcquire(self->lock);
          self->streamBuffer = node->data;
          self->streamBufferSize = node->len;
          CoreLockRelease(self->lock);
          IoDecodeFeed(self->decodeJob, self->streamBufferSize, self->streamBuffer);
        }
      }
    }
    if (!pending) {
      IoDiscFinishRequest(IoDiscGetManager());
      CoreLockAcquire(self->lock);
      if (self->decodeJob != NULL) {
        self->buffer = IoDecodeGetOutput(self->decodeJob);
        IoDecodeCancel(self->decodeJob);
      }
      self->decodeJob = NULL;
      CoreLockRelease(self->lock);
      IoDataClearFlags(self, 10);
      IoDataSetState(self, 9);
      self->statusFlags |= 10;
    }
    break;

  case 9:
    self->done = 1;
    busy = 0;
    break;

  default:
    break;
  }
  return busy;
}
