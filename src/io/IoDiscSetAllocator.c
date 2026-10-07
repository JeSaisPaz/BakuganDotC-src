// bdc 0x089f9b3c IoDiscSetAllocator
#include "bdc.h"

/* Sets the destination-buffer allocator object (`+0xf0`) of the `CODiscSimple` disc reader
   (`g_discSimple`) under its lock; `IoDiscSimpleStateGetSize` calls its virtual slot to
   allocate the file buffer. */

void IoDiscSetAllocator(IoDiscSimple *self, void *allocator)

{
  CoreLockAcquire(self->lock);
  self->allocator = allocator;
  CoreLockRelease(self->lock);
  return;
}

