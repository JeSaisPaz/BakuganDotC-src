// bdc 0x08a23ce0 SndWaveSetChannelDataLen
#include "bdc.h"

/* Sets the sample count of output channel `channel` (0..3) with `sceAudioSetChannelDataLen`.
   Returns `0x80440010` for a bad channel and `0x80440011` when `samples` is not a multiple of 64 in
   64..0xffc0. */

s32 SndWaveSetChannelDataLen(u32 channel, u32 samples)

{
  s32 result;

  result = -0x7fbbfff0;
  if (channel < 4) {
    result = -0x7fbbffef;
    if (samples - 0x40 < 0xff81) {
      result = -0x7fbbffef;
      if ((samples & 0x3f) == 0) {
        result = sceAudioSetChannelDataLen(g_sndWaveChannelIds[channel],samples);
      }
    }
  }
  return result;
}

