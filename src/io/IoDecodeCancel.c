// bdc 0x089fdee4 IoDecodeCancel
#include "bdc.h"

/* Sets the cancel/delete flag (`+0x25`) of a decode job (`CODecode`, 0x10b0 bytes); the decode
   thread deletes it on its next pass (`IoDecodeMngUpdate`). */

void IoDecodeCancel(IoDecodeJob *self)

{
  CoreLockAcquire(self->lock);
  self->cancelled = '\x01';
  CoreLockRelease(self->lock);
  return;
}

