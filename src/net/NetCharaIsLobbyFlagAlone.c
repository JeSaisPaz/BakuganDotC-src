// bdc 0x089d06e8 NetCharaIsLobbyFlagAlone
#include "bdc.h"

/* Returns 1 when the character's header flag bit 0 (`+0x34`, `NetCharaSetLobbyFlag`) is set and
   the NetPlay session has at most one peer (`NetPlayGetPeerCount` ≤ 1), else 0. Used by
   `UiNetLobbyHostPhase`. */

int NetCharaIsLobbyFlagAlone(NetChara *self)

{
  int result = 0;

  CoreLockAcquire(self->lock);
  if (((self->outHdr).flags & 1) != 0) {
    result = 1;
    if (NetPlayHasManager()) {
      if (1 < NetPlayGetPeerCount(NetPlayGetManager())) {
        result = 0;
      }
    }
  }
  CoreLockRelease(self->lock);
  return result;
}

