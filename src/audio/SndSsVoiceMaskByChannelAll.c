// bdc 0x08a23198 SndSsVoiceMaskByChannelAll
#include "bdc.h"

/* Like `SndSsVoiceMaskByChannel` but also includes keyed-off voices (state 5): returns the mask
   (bit n = voice n) of voices in state 3, 7 or 5 on channel `channel` carrying `handle` that have
   a pending change (`g_sndSsVoicePendingMask`) or whose SAS end flag is clear. Returns 0 when
   the voice layer is down (`g_sndSsVoiceState` == -1) or `channel` >= 16. */

u32 SndSsVoiceMaskByChannelAll(u32 channel, s32 handle)
{
  u32 bit;
  u32 mask;
  u32 endFlags;
  u32 i;
  SndSsVoice *voice;

  bit = 1;
  mask = 0;
  if (g_sndSsVoiceState != -1 && channel < 16) {
    endFlags = SndSasGetEndFlag();
    i = 0;
    if (g_sndSsVoiceCount != 0) {
      voice = g_sndSsVoices;
      do {
        if (voice->state == 3 || voice->state == 7 || voice->state == 5) {
          if (voice->channel == channel && voice->handle == handle) {
            if ((bit & g_sndSsVoicePendingMask) != 0) {
              mask |= bit;
            } else {
              mask |= bit & ~endFlags;
            }
          }
        }
        i++;
        bit <<= 1;
        voice++;
      } while (i < g_sndSsVoiceCount);
    }
  }
  return mask;
}
