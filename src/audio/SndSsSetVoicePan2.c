// bdc 0x08a20bfc SndSsSetVoicePan2
#include "bdc.h"

/* Sets the second (effect-side) pan byte of a voice (`voice+0x63`, 0..0x7f) under the layer mutex
   via `SndSsVoiceSetPan2`. Returns `0x80450001` when the layer is not initialised. */

s32 SndSsSetVoicePan2(s32 voiceId, u32 pan)
{
  s32 result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
    result = SndSsVoiceSetPan2(voiceId,pan);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return result;
}
