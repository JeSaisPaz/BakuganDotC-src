// bdc 0x08a23470 SndSsVoiceSetPitchOffset
#include "bdc.h"

/* Stores a pitch offset for voice `voice` (`+0x64`); `SndSsVoiceUpdateParams` adds it to the note
   pitch (clamped 1..0x4000) when it changes. Returns 0, `0x80450011` bad voice, `0x80450001` when
   the voice layer is down. */

s32 SndSsVoiceSetPitchOffset(u32 voice, s32 offset)
{
  if (g_sndSsVoiceState == -1) {
    return (s32)0x80450001;
  }
  if (voice >= g_sndSsVoiceCount) {
    return (s32)0x80450011;
  }
  g_sndSsVoices[voice].field64 = (u32)offset;
  return 0;
}
