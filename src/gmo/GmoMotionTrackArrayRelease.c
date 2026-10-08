// bdc 0x08a1467c GmoMotionTrackArrayRelease
#include "bdc.h"

/* Drops one reference on each of `n` consecutive 0x10-byte GMO motion track records; a track
   reaching 0 releases its key data (`+4`) and itself (`GmoHeapRelease`). Returns `arr`. */

short *GmoMotionTrackArrayRelease(short *arr, int n)
{
  short *ptr;
  int i;

  if (arr != NULL) {
    ptr = arr;
    for (i = 0; i < n; i++) {
      *ptr = *ptr - 1;
      if (*ptr == 0) {
        GmoHeapReleaseThunk(0, PspPtr(((GmoMotionTrack *)ptr)->data));
        GmoHeapReleaseThunk(0, ptr);
      }
      ptr += 8;
    }
  }
  return arr;
}
