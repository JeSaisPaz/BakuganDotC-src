// bdc 0x08a22768 SndSsSynthesizeWithMix
#include "bdc.h"

/* Mixing variant of `SndSsSynthesize`: same voice update and envelope copy around
   `SndSasCoreWithMix`. Returns 0 or -1. */

s32 SndSsSynthesizeWithMix(void *inout, s32 leftVol, s32 rightVol)

{
  s32 ret = -1;
  s32 rc;
  s32 heights[32];
  s32 *src = heights;

  if (g_sndSsVoiceState != -1) {
    SndSsVoiceUpdateParams();
    sceKernelLockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1, (u32 *)0);
    rc = SndSasCoreWithMix(inout, leftVol, rightVol);
    g_sndSsVoicePendingMask = 0;
    sceKernelUnlockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1);
    ret = -1;
    if (-1 < rc) {
      SndSasGetAllEnvelopeHeights(heights);
      if (0 < (s32)g_sndSsVoiceCount) {
        SndSsVoice *v = g_sndSsVoices;
        s32 n = g_sndSsVoiceCount;
        do {
          v->field04 = *src;
          n = n - 1;
          src = src + 1;
          v = v + 1;
        } while (n != 0);
      }
      ret = 0;
    }
  }
  return ret;
}
