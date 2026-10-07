// bdc 0x089d0b9c NetCharaReadSlot
#include "bdc.h"

/* Copies record `idx` (0x28 bytes, ten words) of the front frame of `g_netCharaSlots` into `out`
   and flushes the D-cache for both buffers, under the net character's lock; returns 1 when the
   character has a queued frame (`readyFrames != 0`), else 0. When the ad-hoc manager exists, `idx`
   0/1 picks the local or peer record, swapped (`1 - idx`) when the connection `role` is not 2;
   `-1` and `-2` force record 1 / 0 without the swap. With `out == NULL` nothing is copied but the
   frame is still marked read. The first record read also initialises `firstReadSeq` (while it is
   negative) with the record's first word, and the first call per frame sets the `ready` byte and
   bumps `readCount`. */

bool NetCharaReadSlot(NetChara *self, s32 idx, u32 *out)
{
  bool result;
  NetAdhocConn *conn;
  u32 *rec;
  s32 i;

  result = false;
  CoreLockAcquire(self->lock);
  if (self->readyFrames != 0) {
    if (NetAdhocHasManager()) {
      if (idx == -1) {
        idx = 1;
      } else if (idx == -2) {
        idx = 0;
      } else {
        conn = (NetAdhocConn *)NetAdhocGetManager();
        if (conn->role != 2) {
          idx = 1 - idx;
        }
      }
    }
    if (out != NULL) {
      sceKernelDcacheWritebackInvalidateRange((u32 *)g_netCharaSlots + idx * 10, 0x28);
      rec = (u32 *)g_netCharaSlots + idx * 10;
      for (i = 0; i < 10; i++) {
        out[i] = rec[i];
      }
      sceKernelDcacheWritebackInvalidateRange(out, 0x28);
      if (self->firstReadSeq < 0) {
        self->firstReadSeq = (s32)out[0];
      }
    }
    result = true;
    if (!self->ready) {
      self->ready = 1;
      self->readCount++;
    }
  }
  CoreLockRelease(self->lock);
  return result;
}
