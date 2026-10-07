// bdc 0x08a20928 SndSsStopVoice
#include "bdc.h"

/* Stops one hardware voice of the Sony sound layer: under the layer mutex it reads the SAS
   pause-flag mask (`SndSasGetPauseFlag`) and calls `SndSsVoiceKeyOff(voiceId, paused)` with the
   voice's pause bit. Returns SndSsVoiceKeyOff's result (0 or its error code, e.g. 0x80450013 for a
   paused voice), or `0x80450001` when the layer is not initialised. This is the handler of the
   stop command (`cmdId = -1`) in `SndManagerProcessCommands`. */

s32 SndSsStopVoice(u32 voiceId)

{
  u32 pauseMask;
  s32 result;
  
  result = (s32)0x80450001;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
    pauseMask = SndSasGetPauseFlag();
    result = SndSsVoiceKeyOff(voiceId,pauseMask >> (voiceId & 0x1f) & 1);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return result;
}

