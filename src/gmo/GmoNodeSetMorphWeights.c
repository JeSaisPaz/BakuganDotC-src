// bdc 0x08a15aa4 GmoNodeSetMorphWeights
#include "bdc.h"

/* Sets the morph weights of a 0xc0-byte GMO node record: copies up to 8 words from `src` into the
   weight list `+0xc` (count `+0x1a`), allocating a new pool-0 block (`GmoHeapAlloc`, flag
   0x40000) when the current one is too small; NULL/0 releases the list. */

void GmoNodeSetMorphWeights(GmoNode *self, const float *src, int count)

{
  float *buf;
  
  if (8 < count) {
    count = 8;
  }
  if (self != (GmoNode *)0x0) {
    if ((src != (float *)0x0) && (count != 0)) {
      if ((int)(uint)self->morphCount < count) {
        buf = GmoHeapAlloc(0,0x10,count << 2);
        self->morphWeights = buf;
        if (buf == (float *)0x0) {
          return;
        }
        self->flags = self->flags | 0x40000;
      }
      memcpy(self->morphWeights,src,count << 2);
      self->morphCount = (u16)count;
      return;
    }
    GmoHeapReleaseThunk(0,self->morphWeights);
    self->morphWeights = (float *)0x0;
    self->flags = self->flags & 0xfffbffff;
    self->morphCount = 0;
  }
  return;
}

