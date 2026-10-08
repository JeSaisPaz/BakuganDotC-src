// bdc 0x089de110 GmoMotionUpdateLinked
#include "bdc.h"

/* Updates the `textureCount` texture records (`GmoLayer`, 16 bytes each at `textures`) of a
   `GmoModel` with `GmoTextureAnimUpdate(dt, layer->texture, flags)`, where `flags` keeps bits 0
   and 1 of `mask`. */

void GmoMotionUpdateLinked(float dt, void *player, u32 mask)

{
  GmoModel *p = (GmoModel *)player;
  u32 flags;
  int i;

  flags = (u32)((mask & 1) != 0);
  if ((mask & 2) != 0) {
    flags = flags | 2;
  }
  for (i = 0; i < (int)p->textureCount; i++) {
    GmoTextureAnimUpdate(dt,((GmoLayer *)p->textures)[i].texture,flags);
  }
}
