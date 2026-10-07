// bdc 0x08a20adc SndSsSetVoicePan
#include "bdc.h"

/* Sets the pan (0..0x7f, 0x40 = centre) of a hardware voice of the Sony sound layer under its mutex
   (`SndSsVoiceSetPan(voiceId, pan)`); returns its result, or `0x80450001` when the layer is not
   initialised. `SndManagerProcessCommands` calls it for the set-pan command (`cmdId = -5`, see
   `SndManagerSetPan`). */

s32 SndSsSetVoicePan(s32 voiceId, u32 pan)
{
  s32 result;
  
  result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
    result = SndSsVoiceSetPan(voiceId,pan);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return result;
}

