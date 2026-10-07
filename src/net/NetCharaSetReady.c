// bdc 0x089d0d08 NetCharaSetReady
#include "bdc.h"

/* Sets the 'frame ready' byte (`+5`) of a net character under its lock. `NetCharaCommitFrame`
   only advances the slot queue while this byte is set (and clears it again). Called by
   `NetPlayLateUpdate` when the 'synced' byte is not set, and by `BtlHudArenaResultScreen` / `BtlHudUpdateResultScreen`
   (game code). */

void NetCharaSetReady(NetChara *self)

{
  CoreLockAcquire(self->lock);
  self->ready = '\x01';
  CoreLockRelease(self->lock);
  return;
}

