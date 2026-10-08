// bdc 0x08a1470c GmoMotionArrayRelease
#include "bdc.h"

/* Drops one reference on each of `n` consecutive 0x30-byte GMO motion records; a motion reaching 0
   releases its track array (`+4`, `+0xc` tracks of 0x10 bytes, each ref-counted with its key data
   at `+4`), its track table (`+8`) and itself (`GmoHeapRelease`). Returns `arr`. */

short *GmoMotionArrayRelease(short *arr, int n)

{
  GmoMotionInfo *motion;
  GmoMotionTrack *track;
  int i;
  int t;

  if (arr != (short *)0x0) {
    motion = (GmoMotionInfo *)arr;
    for (i = 0; i < n; i++, motion++) {
      motion->active--;
      if (motion->active == 0) {
        track = (GmoMotionTrack *)PspPtr(motion->tracks);
        if (track != (GmoMotionTrack *)0x0) {
          for (t = 0; t < (int)motion->trackCount; t++, track++) {
            track->flags--;
            if (track->flags == 0) {
              GmoHeapReleaseThunk(0,PspPtr(track->data));
              GmoHeapReleaseThunk(0,track);
            }
          }
        }
        GmoHeapReleaseThunk(0,PspPtr(motion->table));
        GmoHeapReleaseThunk(0,motion);
      }
    }
  }
  return arr;
}
