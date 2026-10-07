// bdc 0x08a20724 SndSsRenderBlock
#include "bdc.h"

/* Public per-block entry of the Sony sound layer: advances the sequencer (`SndSsSeqTick`) and
   synthesizes one SAS grain into `out` (`SndSsSynthesize`). Returns 0, or `0x80450001` when the
   layer is not initialised. */

s32 SndSsRenderBlock(void *out)
{
  s32 result = -0x7fbaffff;
  if (g_sndSsState != -1) {
    SndSsSeqTick();
    SndSsSynthesize(out);
    result = 0;
  }
  return result;
}
