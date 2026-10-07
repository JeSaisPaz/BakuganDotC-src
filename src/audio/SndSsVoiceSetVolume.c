// bdc 0x08a232a4 SndSsVoiceSetVolume
#include "bdc.h"

/* Stores the volume (0..0x7f) of voice `voice` (`+0x60`); `SndSsVoiceUpdateParams` applies it
   before the next grain. Returns 0, `0x80450011` bad voice, `0x8045000a` bad value, `0x80450001`
   when the voice layer is down. */


s32 SndSsVoiceSetVolume(u32 voice, u32 volume)
{
  s32 result;

  result = -0x7fbaffff;
  if (g_sndSsVoiceState != -1) {
    result = -0x7fbaffef;
    if ((voice < g_sndSsVoiceCount) && (result = -0x7fbafff6, volume < 0x80)) {
      result = 0;
      g_sndSsVoices[voice].volume = (u8)volume;
    }
  }
  return result;
}
