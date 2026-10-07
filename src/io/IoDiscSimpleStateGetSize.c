// bdc 0x089fa8c0 IoDiscSimpleStateGetSize
#include "bdc.h"

/* State 2 handler of the `CODiscSimple` reader (vtable slot `+0x24`): asks the file size with
   `sceIoIoctl(fd, 0x01020007, ..)` into a 64-bit value preset to `g_zeroFileOffsetB`; error
   `0x80020323` restarts at state 1 (result cleared), any other error or a size not above the preset
   leaves the state unchanged. With a size it stores the low word in `size` and goes to close
   (state 6) when the request is size-only (`mode == 2`) or has no destination (`dest == -1`).
   Otherwise it resolves `dest`: when `destResolved` is still set from an earlier pass it releases
   that buffer (allocator entry 2, or `MemFree` when there is no allocator) and restores the 0/1
   request from `destFromLow`; else it records the 0/1 request in `destFromLow`. A streamed request
   then goes to state 3. Otherwise the buffer comes from the allocator (entry 1; on failure with
   `abortFlag` set the reader goes idle, state 0) or, for `dest` 0 / 1, from the game heap
   allocating from high / low addresses; a non-NULL `dest` sets `destResolved` and state 4, a NULL
   one leaves the state unchanged so the step is retried. */

typedef void (*IoDiscAllocatorFreeFn)(void *self, void *ptr);
typedef void *(*IoDiscAllocatorAllocFn)(void *self, u32 size, int arg2, int arg3);

void IoDiscSimpleStateGetSize(IoDiscSimple *self)
{
  SceOff zero;
  SceOff fileSize;
  int result;
  void *dest;
  void *allocator;
  const VtblEntry *entry;
  u8 streamed;
  u8 fromLow;
  bool prevFromLow;
  u32 size;

  zero = g_zeroFileOffsetB;
  fileSize = zero;
  result = sceIoIoctl(self->fd, 0x01020007, NULL, 0, &fileSize, 8);
  self->result = result;
  if (result < 0) {
    if (result == (int)0x80020323) {
      self->result = 0;
      self->asyncPending = 0;
      self->state = 1;
    }
    return;
  }
  if (!((u64)fileSize > (u64)zero)) {
    return;
  }
  self->size = (u32)fileSize;
  if (self->mode == 2) {
    self->state = 6;
    return;
  }
  dest = self->dest;
  if (dest == (void *)(intptr_t)-1) {
    self->state = 6;
    return;
  }
  if (fileSize == zero) {
    self->state = 6;
    return;
  }

  if (self->destResolved != 0) {
    allocator = self->allocator;
    if (allocator != NULL) {
      entry = &(*(const VtblEntry **)allocator)[2];
      ((IoDiscAllocatorFreeFn)entry->fn)((u8 *)allocator + entry->delta, dest);
    } else if (dest != NULL) {
      MemLock();
      MemFree(dest, NULL, 0);
      MemUnlock();
      self->dest = NULL;
    }
    fromLow = self->destFromLow;
    streamed = self->streamed;
    if (fromLow == 0) {
      self->dest = NULL;
    } else if (fromLow == 1) {
      self->dest = (void *)(intptr_t)1;
    }
  } else {
    streamed = self->streamed;
    if (dest == NULL) {
      self->destFromLow = 0;
    } else if (dest == (void *)(intptr_t)1) {
      self->destFromLow = 1;
    }
  }

  if (streamed != 0) {
    self->state = 3;
    return;
  }

  allocator = self->allocator;
  if (allocator != NULL) {
    entry = &(*(const VtblEntry **)allocator)[1];
    dest = ((IoDiscAllocatorAllocFn)entry->fn)((u8 *)allocator + entry->delta, self->size, 0, 0);
    self->dest = dest;
    if (dest == NULL && self->abortFlag != 0) {
      self->state = 0;
      self->idle = 1;
      return;
    }
  } else {
    dest = self->dest;
    if (dest == NULL) {
      size = self->size;
      MemLock();
      prevFromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(false);
      dest = MemAlloc(size, NULL, 0);
      MemSetAllocFromLow(prevFromLow);
      MemUnlock();
      self->dest = dest;
    } else if (dest == (void *)(intptr_t)1) {
      size = self->size;
      MemLock();
      prevFromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      dest = MemAlloc(size, NULL, 0);
      MemSetAllocFromLow(prevFromLow);
      MemUnlock();
      self->dest = dest;
    }
  }
  if (dest != NULL) {
    self->destResolved = 1;
    self->state = 4;
  }
}
