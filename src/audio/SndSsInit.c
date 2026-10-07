// bdc 0x08a1fd00 SndSsInit
#include "bdc.h"

/* Initialises the statically linked Sony sound layer (the library behind the `sceSs*` messages; its
   lock is the LwMutex `"SceLibsndpSmfExcl"`): creates that LwMutex, runs the low-level setup
   (`SndSasInit(bufferSize)`, `SndSsSeqInit(bufferSize)`, `SndSsVoiceInit(maxVoices)`), clears the
   0x80-entry bank table (`g_sndSsBankTable`), registers the 32 channel defaults
   (`SndSsSetVoiceVelocity(i, 0x7f)`), stores the voice count in `g_sndSsMaxVoices` and sets the
   layer's state to 'initialised' (`g_sndSsState = 0`). Returns 0 on success; a negative
   `sceKernelCreateLwMutex` result is returned as is (nothing else touched); a nonzero
   `SndSasInit`/`SndSsVoiceInit` result is returned after deleting the mutex again and marking the
   layer unusable (`g_sndSsState = -1`). */

s32 SndSsInit(s32 maxVoices, s32 bufferSize)
{
  s32 ret;
  s32 i;

  ret = sceKernelCreateLwMutex((SceLwMutexWorkarea *)&g_sndSsMutex, "SceLibsndpSmfExcl", 0, 0,
                               NULL);
  if (ret < 0) {
    return ret;
  }
  ret = SndSasInit(bufferSize);
  if (ret == 0) {
    SndSsSeqInit(bufferSize);
    ret = SndSsVoiceInit(maxVoices);
    if (ret == 0) {
      for (i = 0; i < 0x80; i++) {
        g_sndSsBankTable[i] = 0;
      }
      for (i = 0; i < 0x20; i++) {
        SndSsSetVoiceVelocity(i, 0x7f);
      }
      g_sndSsMaxVoices = maxVoices;
      g_sndSsState = 0;
      return 0;
    }
  }
  sceKernelDeleteLwMutex((SceLwMutexWorkarea *)&g_sndSsMutex);
  g_sndSsState = -1;
  return ret;
}
