// bdc 0x089d05d8 NetCharaAppendOutData
#include "bdc.h"

/* Appends `len` bytes of `data` to the character's outgoing payload (`+0xf0`, length `+0xf4`) if it
   still fits in 0x800 bytes. Returns 1 on success, 0 otherwise. */

int NetCharaAppendOutData(NetChara *self, void *data, int len)

{
  int ok;

  ok = 0;
  CoreLockAcquire(self->lock);
  if (((data != (void *)0x0) && (0 < len)) && (self->outLen + len < 0x800)) {
    memcpy(self->outBuf + self->outLen,data,len);
    ok = 1;
    self->outLen = self->outLen + len;
  }
  CoreLockRelease(self->lock);
  return ok;
}

