// bdc 0x089d0934 NetCharaSlotsHaveFlag
#include "bdc.h"

/* Returns 1 when either record of the front frame of `g_netCharaSlots` (local `+4`, peer `+0x2c`)
   has any bit of `mask` set, evaluated under the character's lock; else 0. Counterpart of
   `NetCharaBothHaveFlags`; used by `BtlMainUpdateQuitPrompt`. */

int NetCharaSlotsHaveFlag(NetChara *self, u32 mask)
{
  u32 *slots;
  int result = 0;

  CoreLockAcquire(self->lock);
  slots = (u32 *)g_netCharaSlots;
  if ((slots[1] & mask) != 0 || (slots[0xb] & mask) != 0) {
    result = 1;
  }
  CoreLockRelease(self->lock);
  return result;
}
