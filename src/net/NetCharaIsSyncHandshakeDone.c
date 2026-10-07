// bdc 0x089d000c NetCharaIsSyncHandshakeDone
#include "bdc.h"

/* Returns 1 when the local character has queued frames (`NetCharaGetReadyFrames` > 0) and both
   records of the front frame of `g_netCharaSlots` carry flags `0x1000000` and `0x800000`, after a
   countdown in `+0x11c` has run out (it is decremented on each successful check while > 1). Used by
   menus and `NetBattleSyncTaskUpdate` to see when both players reached the same sync point. */

int NetCharaIsSyncHandshakeDone(void)
{
  NetCharaListNode *node;
  NetChara *self;
  u32 *slots;
  int result = 0;

  if (g_netCharaMgr != NULL && g_netCharaMgr->lock != NULL) {
    CoreLockAcquire(g_netCharaMgr->lock);
    node = NetCharaListFirst(g_netCharaMgr->list);
    if (node != NULL && (self = node->chara) != NULL) {
      CoreLockAcquire(self->lock);
      slots = (u32 *)g_netCharaSlots;
      if (NetCharaGetReadyFrames(self) > 0 && (slots[1] & 0x1000000) != 0 &&
          (slots[0xb] & 0x1000000) != 0 && (slots[1] & 0x800000) != 0 &&
          (slots[0xb] & 0x800000) != 0) {
        if (self->syncCountdown < 2) {
          result = 1;
        } else {
          self->syncCountdown = self->syncCountdown - 1;
        }
      }
      CoreLockRelease(self->lock);
    }
    CoreLockRelease(g_netCharaMgr->lock);
  }
  return result;
}
