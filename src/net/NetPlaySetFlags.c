// bdc 0x0881b5e0 NetPlaySetFlags
#include "bdc.h"

/* Sets the `NetPlay` flag word `+0xbc` to `flags` (read by `NetPlayGetFlags`).
   `NetBattleSyncTaskUpdate` steps it through the handshake values 0x8000000, 0x9000000,
   0x4000000. */

void NetPlaySetFlags(NetPlay *self, u32 flags)

{
  if (self->flags != flags) {
    self->flags = flags;
  }
  return;
}

