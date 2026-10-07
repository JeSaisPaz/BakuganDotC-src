// bdc 0x08a20390 SndSsStopHandle
#include "bdc.h"

/* Stops every hardware voice of the Sony sound layer that was started with `handle` (the voice mask
   comes from `SndSsVoiceMaskByHandle(handle)`, each voice is stopped with `SndSsVoiceKeyOff`)
   under the layer mutex. Returns the mask of the voices that were stopped successfully (0 when the
   layer is not initialised). `SndManagerUpdateVoices` calls it when a voice's 0.125 s fade-out
   has run out. */

u32 SndSsStopHandle(s32 handle)

{
  u32 pauseMask;
  u32 voiceMask;
  u32 active;
  s32 res;
  u32 stopped;
  u32 paused;
  u32 voice;
  u32 nextVoice;
  u32 bit;
  
  stopped = 0;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,(u32 *)0x0);
    pauseMask = SndSasGetPauseFlag();
    voiceMask = SndSsVoiceMaskByHandle(handle);
    bit = 1;
    stopped = 0;
    voice = 0;
    if (g_sndSsMaxVoices != 0) {
      do {
        paused = pauseMask & 1;
        active = voiceMask & 1;
        nextVoice = voice + 1;
        pauseMask = pauseMask >> 1;
        if (voiceMask == 0) break;
        voiceMask = voiceMask >> 1;
        if (active != 0) {
          res = SndSsVoiceKeyOff(voice,paused);
          if (res == 0) {
            stopped = stopped | bit;
          }
        }
        bit = bit << 1;
        voice = nextVoice;
      } while (nextVoice < (uint)g_sndSsMaxVoices);
    }
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return stopped;
}

