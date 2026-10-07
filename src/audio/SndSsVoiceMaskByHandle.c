// bdc 0x08a23014 SndSsVoiceMaskByHandle
#include "bdc.h"

/* Returns the mask of sounding voices (state 3 or 7) that were started with `handle`: bit i is set
   for voice i of `g_sndSsVoices`. Returns 0 when the voice layer is unusable
   (`g_sndSsVoiceState == -1`) or there are no voices. */

u32 SndSsVoiceMaskByHandle(s32 handle)

{
  SndSsVoice *voice;
  u32 mask;
  u32 bit;
  u32 i;
  u32 count;

  bit = 1;
  mask = 0;
  if (g_sndSsVoiceState != -1 && (count = g_sndSsVoiceCount) != 0) {
    voice = g_sndSsVoices;
    i = 0;
    do {
      i++;
      if (((s8)voice->state == 3 || (s8)voice->state == 7) && voice->handle == handle) {
        mask |= bit;
      }
      bit <<= 1;
      voice++;
    } while (i < count);
  }
  return mask;
}
