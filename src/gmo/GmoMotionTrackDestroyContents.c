// bdc 0x08a142d4 GmoMotionTrackDestroyContents
#include "bdc.h"

/* Releases the key-data block (`+4`, `GmoHeapRelease`, pool 0) of a 0x10-byte GMO motion track
   (f-curve) record without freeing the record. Returns the record. */

GmoMotionTrack *GmoMotionTrackDestroyContents(GmoMotionTrack *track)

{
  if (track != NULL) {
    GmoHeapReleaseThunk(0,track->data);
  }
  return track;
}

