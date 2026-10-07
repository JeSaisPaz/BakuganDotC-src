// bdc 0x08a20b6c SndSsSetVoiceVolume2
#include "bdc.h"

/* Sets the second (effect-side) volume byte of a voice (`voice+0x62`, 0..0x7f) under the layer
   mutex via `SndSsVoiceSetVolume2`. Returns `0x80450001` when the layer is not initialised. */

s32 SndSsSetVoiceVolume2(s32 voiceId, u32 volume)
{
  s32 result;
  
  result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
    result = SndSsVoiceSetVolume2(voiceId,volume);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return result;
}

