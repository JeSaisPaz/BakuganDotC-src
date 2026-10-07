// bdc 0x08a23d50 SndWaveOutputPannedBlocking
#include "bdc.h"

/* Outputs `buf` on channel `channel` (0..3) with `sceAudioOutputPannedBlocking`. Returns
   `0x80440010` for a bad channel and `0x8044000a` for a volume above 0x8000. */

s32 SndWaveOutputPannedBlocking(u32 channel, u32 leftVol, u32 rightVol, void *buf)

{
  s32 result;

  result = -0x7fbbfff0;
  if (channel < 4) {
    result = -0x7fbbfff6;
    if (leftVol < 0x8001) {
      result = -0x7fbbfff6;
      if (rightVol < 0x8001) {
        result = sceAudioOutputPannedBlocking(g_sndWaveChannelIds[channel],leftVol,rightVol,buf);
      }
    }
  }
  return result;
}

