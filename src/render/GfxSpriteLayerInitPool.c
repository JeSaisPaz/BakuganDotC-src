// bdc 0x089f5240 GfxSpriteLayerInitPool
#include "bdc.h"

/* Allocates a pool of `count` sprites (`count * 0x160` bytes, from the low end of the heap, under
   MemLock) for the layer, zeroes it, and initialises each slot (CoreObjectClearLinks node reset,
   GfxSpriteInit, `slotFlags = 1` = lives in the pool, `poolIndex = i`). Stores the pool in `pool`,
   its size in `poolCount` and clears `poolLast`/`poolUsed`. */

void GfxSpriteLayerInitPool(GfxSpriteLayer *self, int count)

{
  bool fromLow;
  GfxSprite *pool;
  int i;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = MemAlloc(count * (int)sizeof(GfxSprite), (const char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->pool = pool;
  memset(pool, 0, count * sizeof(GfxSprite));
  self->poolCount = count;
  self->poolLast = (GfxSprite *)0x0;
  self->poolUsed = 0;
  for (i = 0; i < count; i++) {
    CoreObjectClearLinks((CoreObject *)&self->pool[i]);
    GfxSpriteInit(&self->pool[i]);
    self->pool[i].slotFlags = 1;
    self->pool[i].poolIndex = i;
  }
}
