// bdc 0x08828b20 GfxPuffCreate
#include "bdc.h"

/* Allocates (low heap) and constructs a sprite puff (`GfxPuffCtor`) of kind `kind`
   (`GfxPuffCtor`), sets its position `posX` (+0x60) from `pos` (vec4), adds it to the puff
   sprite layer `g_puffSpriteLayer` (`GfxSpriteLayerAdd`; also stored in `layer`) and runs its
   first update (virtual slot 2). Returns the puff (NULL if the allocation failed, after the
   position store, which then writes through NULL as the original does). */

GfxPuff * GfxPuffCreate(u8 kind, const float *pos)

{
  bool fromLow;
  GfxPuff *alloc;
  GfxPuff *puff;
  const VtblEntry *update;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(sizeof(GfxPuff), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  puff = NULL;
  if (alloc != NULL) {
    GfxPuffCtor(alloc, kind);
    puff = alloc;
  }
  puff->base.posX = pos[0];
  puff->base.posY = pos[1];
  puff->base.posZ = pos[2];
  puff->base.posW = pos[3];
  GfxSpriteLayerAdd(g_puffSpriteLayer, (CoreObject *)puff);
  puff->layer = g_puffSpriteLayer;
  update = &((const VtblEntry *)puff->base.vtable)[2];
  ((void (*)(void *))update->fn)((u8 *)puff + update->delta);
  return puff;
}
