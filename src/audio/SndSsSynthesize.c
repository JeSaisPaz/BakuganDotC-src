// bdc 0x08a2269c SndSsSynthesize
#include "bdc.h"

/* Synthesizes one SAS grain into `out`: pushes changed voice parameters to SAS
   (`SndSsVoiceUpdateParams`), renders with `SndSasCore` under the voice mutex, clears the
   voice-changed mask (`g_sndSsVoicePendingMask`), then copies the 32 envelope heights
   (`SndSasGetAllEnvelopeHeights`) into the voice records (`+4`). Returns 0, or -1 when the voice
   layer is down or SAS fails. */

s32 SndSsSynthesize(void *out)

{
  s32 ret = -1;
  s32 rc;
  s32 heights[32];
  s32 *src = heights;

  if (g_sndSsVoiceState != -1) {
    SndSsVoiceUpdateParams();
    sceKernelLockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1, (u32 *)0);
    rc = SndSasCore(out);
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
