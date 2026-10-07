// bdc 0x08a22598 SndSsVoiceInit
#include "bdc.h"

/* Sets up the voice table (`g_sndSsVoices`, 0x68-byte SndSsVoice records; state 3 keyed on,
   7 key released but held by the sustain pedal, 5 keyed off/free) for `numVoices` (1..32) voices:
   every record is marked free (state 5), reset (`SndSsVoiceReset`), given its SAS voice index
   and put on the free list (`g_sndSsVoiceFreeList`, count `g_sndSsVoiceFreeCount`); then
   creates the voice-layer LwMutex `"SceLibsndpPrevExcl"` (`g_sndSsVoiceMutex`) and marks the
   voice layer up (`g_sndSsVoiceState` = 0, no pending changes). Returns 0, `0x8045000a` for a
   bad count (the layer stays at state -1), or the negative mutex error. */

s32 SndSsVoiceInit(u32 numVoices)
{
  s32 ret;
  u32 i;
  SndSsVoice *voice;
  SndSsVoice **slot;

  g_sndSsVoiceState = -1;
  ret = (s32)0x8045000a;
  if (numVoices - 1 < 32) {
    g_sndSsVoiceCount = numVoices;
    i = 0;
    if (numVoices != 0) {
      slot = g_sndSsVoiceFreeList;
      voice = g_sndSsVoices;
      do {
        voice->state = 5;
        voice->field18 = 0;
        voice->field04 = 0;
        SndSsVoiceReset(voice);
        voice->sasVoice = (u8)i;
        i++;
        *slot = voice;
        voice++;
        slot++;
      } while (i < g_sndSsVoiceCount);
    }
    g_sndSsVoiceFreeCount = g_sndSsVoiceCount;
    ret = sceKernelCreateLwMutex(&g_sndSsVoiceMutex, "SceLibsndpPrevExcl", 0, 0, NULL);
    if (ret >= 0) {
      g_sndSsVoiceState = 0;
      ret = 0;
      g_sndSsVoicePendingMask = 0;
    }
  }
  return ret;
}
