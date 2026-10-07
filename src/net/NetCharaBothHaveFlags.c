// bdc 0x089d08bc NetCharaBothHaveFlags
#include "bdc.h"

/* Returns 1 when both records of the front frame of `g_netCharaSlots` share at least one bit with
   `mask` in their flag word (`slots[1] & mask` and `slots[0xb] & mask`, i.e. the second word of the
   local and the peer record), else 0; evaluated under the character's lock (`+8`). `NetPlayUpdate`
   calls it with `NetPlayGetFlags` as the mask to decide whether both sides have reached the same
   state before it declares the frame ready. */

bool NetCharaBothHaveFlags(NetChara *self, u32 mask)
{
  u32 *slots;
  bool result = false;

  CoreLockAcquire(self->lock);
  slots = (u32 *)g_netCharaSlots;
  if ((slots[1] & mask) != 0 && (slots[0xb] & mask) != 0) {
    result = true;
  }
  CoreLockRelease(self->lock);
  return result;
}
