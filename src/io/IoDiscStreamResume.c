// bdc 0x089f9c0c IoDiscStreamResume
#include "bdc.h"

/* Continues a streamed read of the `CODiscSimple` disc reader (`g_discSimple`): in state 3, when
   the next buffer node (`bufNodes[writeIndex]`) is free, marks it filling, makes it the current buffer
   (`curNode`), advances the write index and switches to state 5 (read chunk), then wakes the disc
   thread (index 2). Returns the wake result (1 on success) if it resumed, otherwise 0. */

int IoDiscStreamResume(IoDiscSimple *self)
{
  IoDiscBufNode *node;
  int idx;
  int woke;

  woke = 0;
  CoreLockAcquire(self->lock);
  if ((self->state == 3) && (node = self->bufNodes[self->writeIndex], node->state == 0)) {
    self->state = 5;
    self->curNode = node;
    node->state = 1;
    self->curNode->len = 0;
    idx = self->writeIndex + 1;
    self->writeIndex = idx;
    if (1 < idx) {
      self->writeIndex = 0;
    }
    woke = 1;
  }
  CoreLockRelease(self->lock);
  if (woke) {
    woke = BootWakeupThread(2);
  }
  return woke;
}
