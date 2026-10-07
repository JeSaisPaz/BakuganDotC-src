// bdc 0x089de110 GmoMotionUpdateLinked
#include "bdc.h"

/* Updates the `+0x1e` linked entries (16 bytes each at `+0x10`) of a motion player with
   `GmoTextureAnimUpdate(dt, entry->obj, flags)`, where `flags` keeps bits 0 and 1 of `mask`. */

void GmoMotionUpdateLinked(float dt, void *player, u32 mask)

{
  GmoMotionPlayer *p = (GmoMotionPlayer *)player;
  u32 flags;
  int i;

  flags = (u32)((mask & 1) != 0);
  if ((mask & 2) != 0) {
    flags = flags | 2;
  }
  for (i = 0; i < (int)p->linkCount; i++) {
    GmoTextureAnimUpdate(dt,p->links[i].obj,flags);
  }
}
