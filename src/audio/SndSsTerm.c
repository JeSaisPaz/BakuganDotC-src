// bdc 0x08a1fe00 SndSsTerm
#include "bdc.h"

/* Shuts the Sony sound layer down: if it is initialised it runs the low-level teardown
   (`SndSasTerm`, `SndSsVoiceTerm`), deletes the layer's LwMutex and marks it uninitialised
   (`g_sndSsState = -1`); returns 0, or the first failing result (`0x80450001` when it was not
   initialised, SndSasTerm's nonzero result, or a negative result of SndSsVoiceTerm / the
   mutex delete). */

s32 SndSsTerm(void)
{
  s32 result;

  if (g_sndSsState == -1) {
    return (s32)0x80450001;
  }
  result = SndSasTerm();
  if (result != 0) {
    return result;
  }
  result = SndSsVoiceTerm();
  if (result < 0) {
    return result;
  }
  result = sceKernelDeleteLwMutex((SceLwMutexWorkarea *)&g_sndSsMutex);
  if (result < 0) {
    return result;
  }
  g_sndSsState = -1;
  return 0;
}
