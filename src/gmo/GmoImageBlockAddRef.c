// bdc 0x08a0ff54 GmoImageBlockAddRef
#include "bdc.h"

/* Increments the reference count of an image-heap block and returns the new count, zero-extended
   (0 for NULL). */

u16 GmoImageBlockAddRef(void *block)
{
  GmoImageBlock *b = (GmoImageBlock *)block;
  u16 count = 0;

  if (b != NULL) {
    count = (u16)(b->refCount + 1);
    b->refCount = (s16)count;
  }
  return count;
}
