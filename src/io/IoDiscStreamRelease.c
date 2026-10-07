// bdc 0x089f9f68 IoDiscStreamRelease
#include "bdc.h"

/* For a streamed read of the `CODiscSimple` disc reader (`g_discSimple`): with `index < 0` frees
   both buffer nodes and resets the stream indices/counters; otherwise frees the filled node at the
   read index and advances it. Returns 1 when something was released. */

int IoDiscStreamRelease(IoDiscSimple *self, int index)

{
  int result;
  int i;
  
  result = 0;
  CoreLockAcquire(self->lock);
  if (self->streamed != '\0') {
    if (index < 0) {
      for (i = 0; i < 2; i++) {
        if (self->bufNodes[i] != (IoDiscBufNode *)0x0) {
          self->bufNodes[i]->state = 0;
        }
      }
      self->writeIndex = 0;
      self->readIndex = 0;
      self->curNode = (IoDiscBufNode *)0x0;
      self->streamBytes = 0;
      result = 1;
    }
    else if (self->bufNodes[self->readIndex]->state == 2) {
      self->bufNodes[self->readIndex]->state = 0;
      self->readIndex = self->readIndex + 1;
      if (1 < self->readIndex) {
        self->readIndex = 0;
      }
      result = 1;
    }
  }
  CoreLockRelease(self->lock);
  return result;
}

