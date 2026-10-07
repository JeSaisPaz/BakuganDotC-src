// bdc 0x08a20774 SndSsRenderBlockWithMix
#include "bdc.h"

/* Like `SndSsRenderBlock` but mixes the SAS output into the samples already in `inout`
   (`SndSsSynthesizeWithMix` → `SndSasCoreWithMix`). Returns 0, or `0x80450001` when the layer
   is not initialised. */

s32 SndSsRenderBlockWithMix(void *inout, s32 leftVol, s32 rightVol)
{
  s32 result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    SndSsSeqTick();
    SndSsSynthesizeWithMix(inout,leftVol,rightVol);
    result = 0;
  }
  return result;
}
