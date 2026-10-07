// bdc 0x089fde2c IoDecodeFeed
#include "bdc.h"

/* Feeds the next input chunk to a streamed decode job (`CODecode`, 0x10b0 bytes): when it waits
   for input, stores the chunk size (`+0x1070`) and pointer (`+0x1064`, if non-NULL), clears the
   waiting flag and wakes the decode thread (`BootWakeupThread(3)`). */

int IoDecodeFeed(IoDecodeJob *self, u32 size, void *chunk)

{
  int result;
  
  result = 0;
  CoreLockAcquire(self->lock);
  if (self->waitingInput != '\0') {
    self->chunkSize = size;
    if (chunk != (void *)0x0) {
      self->chunk = chunk;
    }
    self->waitingInput = '\0';
    result = BootWakeupThread(3);
  }
  CoreLockRelease(self->lock);
  return result;
}

