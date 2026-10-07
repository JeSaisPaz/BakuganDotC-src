// bdc 0x0881b240 NetPlayGetPeer
#include "bdc.h"

/* Returns peer record `i` of the `NetPlay` peer table (0x1f-byte records at `+0x18`:
   name at +0, 6-byte MAC at +0x19), or NULL when `i` is out of range. */

u8 * NetPlayGetPeer(NetPlay *self, s32 i)

{
  if ((-1 < i) && (i < self->peerCount)) {
    return (u8 *)&self->peers[i];
  }
  return (u8 *)0;
}
