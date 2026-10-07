// bdc 0x08a20a4c SndSsSetVoiceVolume
#include "bdc.h"

/* Sets the volume (0..0x7f in the manager's calls) of a hardware voice of the Sony sound layer
   under its mutex (`SndSsVoiceSetVolume(voiceId, volume)`); returns its result, or `0x80450001`
   when the layer is not initialised. Called by `SndManagerProcessCommands` for the set-volume
   command and by `SndManagerUpdateVoices` for the per-frame volume ramp and fade-out. */

s32 SndSsSetVoiceVolume(s32 voiceId, u32 volume)
{
  s32 result;
  
  result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
    result = SndSsVoiceSetVolume(voiceId,volume);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return result;
}

