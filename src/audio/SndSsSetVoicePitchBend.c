// bdc 0x08a2048c SndSsSetVoicePitchBend
#include "bdc.h"

/* Applies a 14-bit MIDI pitch bend (`0x2000` = centre) to a hardware voice: if the layer is
   initialised and `value14 < 0x4000` it converts the value with `SndSsPitchBendToPitch` and
   stores the result as the voice's pitch offset (`SndSsVoiceSetPitchOffset`) under the layer
   mutex. Returns 0x80450001 when not initialised and 0x8045000a for an out-of-range value. */

s32 SndSsSetVoicePitchBend(s32 voiceId, u32 value14)

{
  s32 result;
  
  result = -0x7fbaffff;
  if ((g_sndSsState != -1) && (result = -0x7fbafff6, value14 < 0x4000)) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
    result = SndSsPitchBendToPitch(voiceId,value14);
    result = SndSsVoiceSetPitchOffset(voiceId,result);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return result;
}

