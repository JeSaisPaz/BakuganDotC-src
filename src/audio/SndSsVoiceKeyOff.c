// bdc 0x08a22c34 SndSsVoiceKeyOff
#include "bdc.h"

/* Keys off voice `voice`: a keyed-on voice (state 3) moves from the active list to the free list
   with state 5 and a SAS key-off (`SndSasKeyOff`), or, when its sustain flag (`field18`) is set,
   to the sustained list with state 7 (key-off deferred to the pedal release). Any other non-free
   state is left as is. Then marks the voice changed (`g_sndSsVoicePendingMask |= 1 << voice`, under
   the voice mutex). Returns 0, `0x80450011` bad voice, `0x80450012` voice free (state 0),
   `0x80450013` when the voice is keyed on and `paused` is set; `0x80450001` when the layer is not
   initialised. */

s32 SndSsVoiceKeyOff(u32 voice, s32 paused)
{
  SndSsVoice *v;
  u8 *moved;

  if (g_sndSsVoiceState == -1) {
    return (s32)0x80450001;
  }
  if (voice >= g_sndSsVoiceCount) {
    return (s32)0x80450011;
  }
  v = &g_sndSsVoices[voice];
  if ((s8)v->state == 0) {
    return (s32)0x80450012;
  }
  if ((s8)v->state == 3) {
    if (paused != 0) {
      return (s32)0x80450013;
    }
    if (v->field18 == 0) {
      moved = SndSsVoiceListMove(&g_sndSsVoiceActiveCount, (u8 **)g_sndSsVoiceActiveList,
                                 (s32 *)&g_sndSsVoiceFreeCount, (u8 **)g_sndSsVoiceFreeList, voice);
      moved[1] = 5;
      SndSasKeyOff(moved[0]);
    } else {
      moved = SndSsVoiceListMove(&g_sndSsVoiceActiveCount, (u8 **)g_sndSsVoiceActiveList,
                                 (s32 *)&g_sndSsVoiceSustainedCount,
                                 (u8 **)g_sndSsVoiceSustainedList, voice);
      moved[1] = 7;
    }
  }
  sceKernelLockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1, NULL);
  g_sndSsVoicePendingMask = g_sndSsVoicePendingMask | (1u << (voice & 0x1f));
  sceKernelUnlockLwMutex((SceLwMutex *)&g_sndSsVoiceMutex, 1);
  return 0;
}
