// bdc 0x089d9280 GmoMotionTrackInit
#include "bdc.h"

/* Clears one 0x10-byte motion track record to its defaults: `u16 +0 = 1`, `u16 +2 = 0` (kind
   flags), `void *+4 = NULL` (copied key data), `u16 +8 = 0`, `u16 +0xa = 0`, `u8 +0xc = 0`, `u8
   +0xd = 0`, `u16 +0xe = 0`. */

void GmoMotionTrackInit(GmoMotionTrack *track)

{
  track->flags = 1;
  track->kind = 0;
  track->data = (void *)0x0;
  track->param8 = 0;
  track->paramA = 0;
  track->paramC = '\0';
  track->paramD = '\0';
  track->ref = 0;
  return;
}

