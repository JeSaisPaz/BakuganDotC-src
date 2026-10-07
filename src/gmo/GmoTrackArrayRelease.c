// bdc 0x08a12720 GmoTrackArrayRelease
#include "bdc.h"

/* Drops one reference on each of `n` consecutive texture animation tracks (0x30 bytes); tracks
   reaching 0 are emptied (`GmoTrackDestroyContents`) and freed. */

short *GmoTrackArrayRelease(short *arr, int n)
{
  short *track;
  int i;

  if ((arr != NULL) && (i = 0, track = arr, 0 < n)) {
    do {
      short cnt = *track;
      i = i + 1;
      *track = cnt - 1;
      if ((short)(cnt - 1) == 0) {
        GmoTrackDestroyContents(track);
        GmoImageHeapReleaseThunk(0, track);
      }
      track = track + 0x18;
    } while (n != i);
  }
  return arr;
}
