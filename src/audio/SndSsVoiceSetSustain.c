// bdc 0x08a22d8c SndSsVoiceSetSustain
#include "bdc.h"

/* Sets or releases the sustain (damper) flag of voice `voice`: `on = 1` sets `field18` on a keyed-on
   voice; `on = 0` clears it, and a voice held by the pedal (state 7) is finally keyed off (moved to
   the free list, state 5, `SndSasKeyOff`). Marks the voice changed. Returns 0, `0x80450011` bad
   voice, `0x8045000a` bad `on`, `0x80450012` voice free, `0x80450013` when `paused` is set,
   `0x80450001` when the voice layer is down. */

s32 SndSsVoiceSetSustain(u32 voice, s32 paused, u32 on)
{
  SndSsVoice *v;
  s8 state;

  if (g_sndSsVoiceState == -1)
    return (s32)0x80450001;
  if (voice >= g_sndSsVoiceCount)
    return (s32)0x80450011;
  if (on >= 2)
    return (s32)0x8045000a;
  v = &g_sndSsVoices[voice];
  state = (s8)v->state;
  if (state == 0)
    return (s32)0x80450012;
  if (paused != 0)
    return (s32)0x80450013;

  if (on != 0) {
    if (state == 3)
      v->field18 = on;
  } else if (state == 7) {
    v = (SndSsVoice *)SndSsVoiceListMove(&g_sndSsVoiceSustainedCount,
                                         (u8 **)g_sndSsVoiceSustainedList,
                                         (s32 *)&g_sndSsVoiceFreeCount,
                                         (u8 **)g_sndSsVoiceFreeList, voice);
    v->state = 5;
    v->field18 = 0;
    SndSasKeyOff(v->sasVoice);
  } else if (state == 3) {
    v->field18 = 0;
  }

  sceKernelLockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1, NULL);
  g_sndSsVoicePendingMask |= 1u << voice;
  sceKernelUnlockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1);
  return 0;
}
