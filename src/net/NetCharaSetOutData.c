// bdc 0x089d0554 NetCharaSetOutData
#include "bdc.h"

/* Replaces the character's outgoing payload: copies `len` bytes of `data` to the buffer `+0xf0` and
   sets the length `+0xf4` (NULL `data` just clears the length), under its lock. */

void NetCharaSetOutData(NetChara *self, void *data, int len)

{
  if (data == (void *)0x0) {
    CoreLockAcquire(self->lock);
    self->outLen = 0;
    CoreLockRelease(self->lock);
  }
  else if (0 < len) {
    CoreLockAcquire(self->lock);
    memcpy(self->outBuf,data,len);
    self->outLen = len;
    CoreLockRelease(self->lock);
  }
  return;
}

