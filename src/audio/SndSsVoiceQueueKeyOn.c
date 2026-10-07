// bdc 0x08a229a8 SndSsVoiceQueueKeyOn
#include "bdc.h"

/* Internal key-on of the Sony sound layer (from `SndSsKeyOnTone`, see `SndSsKeyOn`): validates
   the layer (`0x80450001`), voice index (`0x80450011` past `g_sndSsVoiceCount`), voice state
   (`0x80450012` when free, state 0) and `mode == 0` (`0x80450013`). A voice in state 5 (free list)
   or 7 (sustained list) is moved to the active list (`SndSsVoiceListMove`) and set to state 3; a
   voice already in state 3 stays active and is keyed off first (`SndSasKeyOff`, also done for
   state 7). In those three states the 0x58-byte `params` block is copied into the voice record from
   `handle` (+0x10) on and the hardware voice is programmed (`SndSsVoiceApplyKeyOn`). Any other
   state only marks the voice changed. Every accepted call sets the voice's bit in
   `g_sndSsVoicePendingMask` under the voice mutex and returns 0. */

static void SndSsVoiceCopyParams(SndSsVoice *v, const void *params)
{
  u32 *dst = (u32 *)&v->handle;
  const u32 *src = (const u32 *)params;
  s32 i;

  for (i = 0; i < 0x58 / 4; i++)
    dst[i] = src[i];
}

s32 SndSsVoiceQueueKeyOn(u32 voiceId, s32 mode, const void *params)
{
  SndSsVoice *v;
  s8 state;

  if (g_sndSsVoiceState == -1)
    return (s32)0x80450001;
  if (voiceId >= g_sndSsVoiceCount)
    return (s32)0x80450011;
  state = (s8)g_sndSsVoices[voiceId].state;
  if (state == 0)
    return (s32)0x80450012;
  if (mode != 0)
    return (s32)0x80450013;

  if (state == 5) {
    v = (SndSsVoice *)SndSsVoiceListMove(&g_sndSsVoiceFreeCount, (u8 **)g_sndSsVoiceFreeList,
                                         (s32 *)&g_sndSsVoiceActiveCount,
                                         (u8 **)g_sndSsVoiceActiveList, voiceId);
    v->state = 3;
    SndSsVoiceCopyParams(v, params);
    SndSsVoiceApplyKeyOn((u8 *)v);
  } else if (state == 3) {
    v = (SndSsVoice *)SndSsVoiceListMove(&g_sndSsVoiceActiveCount, (u8 **)g_sndSsVoiceActiveList,
                                         (s32 *)&g_sndSsVoiceActiveCount,
                                         (u8 **)g_sndSsVoiceActiveList, voiceId);
    SndSsVoiceCopyParams(v, params);
    SndSasKeyOff(v->sasVoice);
    SndSsVoiceApplyKeyOn((u8 *)v);
  } else if (state == 7) {
    v = (SndSsVoice *)SndSsVoiceListMove(&g_sndSsVoiceSustainedCount,
                                         (u8 **)g_sndSsVoiceSustainedList,
                                         (s32 *)&g_sndSsVoiceActiveCount,
                                         (u8 **)g_sndSsVoiceActiveList, voiceId);
    v->state = 3;
    SndSsVoiceCopyParams(v, params);
    SndSasKeyOff(v->sasVoice);
    SndSsVoiceApplyKeyOn((u8 *)v);
  }

  sceKernelLockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1, NULL);
  g_sndSsVoicePendingMask |= 1u << voiceId;
  sceKernelUnlockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1);
  return 0;
}
