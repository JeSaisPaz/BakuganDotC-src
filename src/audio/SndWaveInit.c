// bdc 0x08a23bb0 SndWaveInit
#include "bdc.h"

/* Initialises the audio-output layer behind `SndSsMixThread` (error space `0x8044xxxx`, likely
   Sony's libwave): points `g_sndWaveDecodePtr`/`g_sndWaveVagPtr` at their scratch buffers,
   clears the voice tables `g_sndWaveVoices`/`g_sndWaveVoiceShadow`, reserves one 0x1c0-sample
   output channel (`g_sndWaveMainChannel`) and three 0x100-sample channels
   (`g_sndWaveChannelIds`[0..2]), then sets `g_sndWaveInitDone`.
   Returns 0 on success; `0x80440001` when already initialised or when a reservation fails. On a
   failed secondary reservation it releases the channels by their loop index (not the reserved id,
   as the original does) and then the main channel. */

s32 SndWaveInit(void)
{
  int ch;
  int i;

  if (g_sndWaveInitDone == 0) {
    g_sndWaveDecodePtr = g_sndWaveDecodeBuf;
    g_sndWaveVagPtr = g_sndWaveVagBlock;
    sceKernelMemset(g_sndWaveVoices, 0, 0x200);
    sceKernelMemset(g_sndWaveVoiceShadow, 0, 0x200);
    g_sndWaveMainChannel = sceAudioChReserve(-1, 0x1c0, 0);
    if (g_sndWaveMainChannel >= 0) {
      for (i = 0; i < 3; i++) {
        ch = sceAudioChReserve(-1, 0x100, 0);
        g_sndWaveChannelIds[i] = ch;
        if (ch < 0) {
          for (i = i - 1; i >= 0; i--) {
            sceAudioChRelease(i);
          }
          sceAudioChRelease(g_sndWaveMainChannel);
          return (s32)0x80440001;
        }
      }
      g_sndWaveInitDone = 1;
      return 0;
    }
  }
  return (s32)0x80440001;
}
