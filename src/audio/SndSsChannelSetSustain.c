// bdc 0x08a20264 SndSsChannelSetSustain
#include "bdc.h"

/* Sustain (damper) pedal for MIDI channel `channel`: under the layer mutex applies
   `SndSsVoiceSetSustain``(voice, pauseBit, on)` to every live voice of that channel and handle
   (`SndSsVoiceMaskByChannel`). Returns the mask of voices for which it returned 0; 0 for
   channel ≥ 16, `on` ≥ 2 or when the layer is down. */

u32 SndSsChannelSetSustain(u32 channel, u32 on, s32 handle)

{
  u32 voices;
  u32 paused;
  u32 bit;
  u32 result;
  u32 voice;

  if (g_sndSsState == -1 || channel >= 0x10 || on >= 2) {
    return 0;
  }
  sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
  voices = SndSsVoiceMaskByChannel(channel,handle);
  paused = SndSasGetPauseFlag();
  bit = 1;
  result = 0;
  voice = 0;
  if (g_sndSsMaxVoices != 0) {
    do {
      if (voices == 0) {
        break;
      }
      if ((voices & 1) != 0 && SndSsVoiceSetSustain(voice,paused & 1,on) == 0) {
        result |= bit;
      }
      voices >>= 1;
      paused >>= 1;
      voice++;
      bit <<= 1;
    } while (voice < (u32)g_sndSsMaxVoices);
  }
  sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  return result;
}
