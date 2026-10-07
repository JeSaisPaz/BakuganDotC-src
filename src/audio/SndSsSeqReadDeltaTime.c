// bdc 0x08a21cd0 SndSsSeqReadDeltaTime
#include "bdc.h"

/* Reads a MIDI variable-length quantity (7 bits per byte, bit 7 = more) from the track's data
   pointer (`+0x1c`, advanced) and stores it as the pending delta time (`+0x34`). Returns 0. */

s32 SndSsSeqReadDeltaTime(SndSsSeqTrack *track)

{
  byte b;
  u32 value;
  
  b = *track->cursor;
  value = (u32)b;
  track->cursor = track->cursor + 1;
  if ((b & 0x80) != 0) {
    value = value & 0x7f;
    do {
      b = *track->cursor;
      track->cursor = track->cursor + 1;
      value = value * 0x80 + (b & 0x7f);
    } while ((s8)b < 0);
  }
  track->delta = value;
  return 0;
}

