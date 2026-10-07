// bdc 0x08a126e0 GmoTrackDestroyContents
#include "bdc.h"

/* Releases the two data blocks (`keys`, `frames`) of a texture animation track (0x30 bytes). Returns `track`. */

void *GmoTrackDestroyContents(void *trackArg)

{
  GmoTexTrack *track = (GmoTexTrack *)trackArg;
  if (track != (GmoTexTrack *)0x0) {
    GmoImageHeapReleaseThunk(0,track->keys);
    GmoImageHeapReleaseThunk(0,track->frames);
  }
  return trackArg;
}
