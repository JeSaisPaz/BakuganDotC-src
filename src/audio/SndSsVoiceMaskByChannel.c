// bdc 0x08a23098 SndSsVoiceMaskByChannel
#include "bdc.h"

/* Returns the mask of sounding voices (state 3 or 7) on MIDI channel `channel` for `handle`,
   skipping voices whose SAS end flag is set unless they have a pending change. */

u32 SndSsVoiceMaskByChannel(u32 channel, s32 handle)
{
  u32 endFlag;
  u32 bit = 1;
  u32 mask = 0;
  u32 i;

  if ((g_sndSsVoiceState != -1) && (channel < 0x10)) {
    endFlag = SndSasGetEndFlag();
    mask = 0;
    for (i = 0; i < g_sndSsVoiceCount; i++, bit <<= 1) {
      SndSsVoice *v = &g_sndSsVoices[i];
      if (((v->state == 3) || (v->state == 7)) && (v->channel == channel) && (v->handle == handle)) {
        if ((bit & g_sndSsVoicePendingMask) == 0) {
          mask |= bit & ~endFlag;
        } else {
          mask |= bit;
        }
      }
    }
  }
  return mask;
}
