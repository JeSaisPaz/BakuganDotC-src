// bdc 0x08a23444 SndSsVoiceTerm
#include "bdc.h"

/* Shuts the voice layer down: marks it unusable (`g_sndSsVoiceState = -1`) and deletes the
   `"SceLibsndpPrevExcl"` mutex, returning `sceKernelDeleteLwMutex`'s result. */

s32 SndSsVoiceTerm(void)

{
  g_sndSsVoiceState = -1;
  return sceKernelDeleteLwMutex((SceLwMutexWorkarea *)&g_sndSsVoiceMutex);
}
