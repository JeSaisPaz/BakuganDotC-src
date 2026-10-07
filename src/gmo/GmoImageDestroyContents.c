// bdc 0x08a12544 GmoImageDestroyContents
#include "bdc.h"

/* Releases what a `GmoImage` record references: its list successor `next` (`+0x4`, one reference,
   `GmoImageArrayRelease`, so freeing a list node frees the rest of the chain), every frame buffer
   (pool 1) and the frame table and user data (pool 0). Returns the record. */

GmoImage *GmoImageDestroyContents(GmoImage *self)

{
  int total;
  int i;

  if (self != (GmoImage *)0x0) {
    GmoImageArrayRelease(&self->next->refCount,1);
    total = (uint)self->levelCount * (uint)self->frameCount;
    for (i = 0; i < total; i++) {
      GmoImageHeapReleaseThunk(1,self->levels[i]);
    }
    GmoImageHeapReleaseThunk(0,self->levels);
    GmoImageHeapReleaseThunk(0,self->userData);
  }
  return self;
}
