// bdc 0x08a209bc SndSsSetVoicePitchOffset
#include "bdc.h"

/* Sets the pitch offset of a hardware voice of the Sony sound layer: under the layer mutex stores
   `offset` with `SndSsVoiceSetPitchOffset` (voice `+0x64`), which `SndSsVoiceUpdateParams` adds
   to the note pitch (clamped 1..0x4000) before the next SAS grain. Returns its result or
   `0x80450001` when the layer is not initialised. */

s32 SndSsSetVoicePitchOffset(s32 voiceId, s32 offset)
{
  s32 result;

  result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex, 1, NULL);
    result = SndSsVoiceSetPitchOffset(voiceId, offset);
    sceKernelUnlockLwMutex(&g_sndSsMutex, 1);
  }
  return result;
}
