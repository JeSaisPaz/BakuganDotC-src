// bdc 0x08a23374 SndSsVoiceSetVolume2
#include "bdc.h"

/* Stores the second (effect-side) volume (0..0x7f) of voice `voice` (`+0x62`), applied by
   `SndSsVoiceUpdateParams`. Same error codes as `SndSsVoiceSetVolume`. */


s32 SndSsVoiceSetVolume2(u32 voice, u32 volume)
{
  s32 result;

  result = -0x7fbaffff;
  if (g_sndSsVoiceState != -1) {
    result = -0x7fbaffef;
    if ((voice < g_sndSsVoiceCount) && (result = -0x7fbafff6, volume < 0x80)) {
      result = 0;
      g_sndSsVoices[voice].volume2 = (u8)volume;
    }
  }
  return result;
}
