// bdc 0x08a233dc SndSsVoiceSetPan2
#include "bdc.h"

/* Stores the second (effect-side) pan (0..0x7f) of voice `voice` (`+0x63`), applied by
   `SndSsVoiceUpdateParams`. Returns 0, `0x8045000a` when `pan >= 0x80`, `0x80450011` bad voice,
   `0x80450001` when the voice layer is down. */

s32 SndSsVoiceSetPan2(u32 voice, u32 pan)
{
  if (g_sndSsVoiceState == -1) {
    return (s32)0x80450001;
  }
  if (voice >= g_sndSsVoiceCount) {
    return (s32)0x80450011;
  }
  if (pan >= 0x80) {
    return (s32)0x8045000a;
  }
  g_sndSsVoices[voice].pan2 = (u8)pan;
  return 0;
}
