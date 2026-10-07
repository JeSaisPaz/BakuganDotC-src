// bdc 0x089f5348 GfxSpriteLayerAllocPoolSlot
#include "bdc.h"

/* Takes a free sprite from the layer's pool: returns NULL when all `+0xc` slots are used (`+0x14`),
   otherwise scans round-robin from the slot after the last one handed out (`+0x10`) for a slot
   without in-use bit 1, resets its node links (`CoreObjectAppend` with NULL), sets bit 1,
   increments the used count and returns it (also stored at `+0x10`). */

GfxSprite *GfxSpriteLayerAllocPoolSlot(GfxSpriteLayer *self)

{
  bool found;
  int poolCount;
  int idx;
  GfxSprite *obj;
  
  poolCount = self->poolCount;
  idx = 0;
  if (poolCount <= self->poolUsed) {
    return (GfxSprite *)0x0;
  }
  if (self->poolLast == (GfxSprite *)0x0) {
    found = 0 < poolCount;
  }
  else {
    idx = self->poolLast->poolIndex + 1;
    found = idx < poolCount;
  }
  if (!found) {
    idx = 0;
  }
  if (idx < poolCount) {
    obj = self->pool + idx;
    do {
      idx = idx + 1;
      if ((obj->slotFlags & 2) == 0) {
        self->poolLast = obj;
        CoreObjectAppend((CoreObject *)obj,(CoreObject *)0x0);
        self->poolLast->slotFlags = self->poolLast->slotFlags | 2;
        self->poolUsed = self->poolUsed + 1;
        return self->poolLast;
      }
      obj = obj + 1;
    } while (idx < poolCount);
  }
  self->poolLast = (GfxSprite *)0x0;
  return (GfxSprite *)0x0;
}

