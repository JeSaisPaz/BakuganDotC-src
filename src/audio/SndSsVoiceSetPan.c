// bdc 0x08a2330c SndSsVoiceSetPan
#include "bdc.h"

/* Stores the pan (0..0x7f) of voice `voice` (`+0x61`), applied by `SndSsVoiceUpdateParams`. Same
   error codes as `SndSsVoiceSetVolume`. */


s32 SndSsVoiceSetPan(u32 voice, u32 pan)
{
  s32 result;

  result = -0x7fbaffff;
  if (g_sndSsVoiceState != -1) {
    result = -0x7fbaffef;
    if ((voice < g_sndSsVoiceCount) && (result = -0x7fbafff6, pan < 0x80)) {
      result = 0;
      g_sndSsVoices[voice].pan = (u8)pan;
    }
  }
  return result;
}
