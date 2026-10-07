// bdc 0x089f9ef8 IoDiscStreamPeek
#include "bdc.h"

/* For a streamed read of the `CODiscSimple` disc reader (`g_discSimple`), returns the buffer node
   at the read index (`+0x34`) when it is filled (node state 2), else NULL. */

void *IoDiscStreamPeek(IoDiscSimple *self)

{
  IoDiscBufNode *node;
  
  CoreLockAcquire(self->lock);
  node = (IoDiscBufNode *)0x0;
  if ((self->streamed != '\0') && (self->bufNodes[self->readIndex]->state == 2)) {
    node = self->bufNodes[self->readIndex];
  }
  CoreLockRelease(self->lock);
  return node;
}

