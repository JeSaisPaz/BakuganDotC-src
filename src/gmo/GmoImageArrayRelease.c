// bdc 0x08a125e4 GmoImageArrayRelease
#include "bdc.h"

/* Drops one reference on each of `n` consecutive `GmoImage` records (0x30 bytes); records
   reaching 0 are destroyed (`GmoImageDestroyContents`, which also releases their list successor
   `next`) and freed. */

short *GmoImageArrayRelease(short *arr, int n)

{
  short count;
  GmoImage *self;
  int i;
  
  if ((arr != (short *)0x0) && (i = 0, self = (GmoImage *)arr, 0 < n)) {
    do {
      i = i + 1;
      count = self->refCount + -1;
      self->refCount = count;
      if (count == 0) {
        GmoImageDestroyContents(self);
        GmoImageHeapReleaseThunk(0,self);
      }
      self = self + 1;
    } while (n != i);
  }
  return arr;
}

